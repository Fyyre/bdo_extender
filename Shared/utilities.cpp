#include "Inc.h"
#include "utilities.h"

#define RtlpIsKeyedEvent(xxHandle) ((((ULONG_PTR)xxHandle) & 1) != 0)
#define RtlpSetKeyedEventHandle(xxHandle) ((HANDLE)(((ULONG_PTR)xxHandle) | 1))

NTSTATUS
RtlpEnterCriticalSection(IN PRTL_CRITICAL_SECTION CriticalSection)
{

    ULONG64 SpinCount;
    HANDLE Thread;

    Thread = NtCurrentTeb()->ClientId.UniqueThread;
    if (Thread == CriticalSection->OwningThread)
    {

        ASSERT(CriticalSection->LockCount >= 0);

        InterlockedIncrement(&CriticalSection->LockCount);
        CriticalSection->RecursionCount += 1;
        return STATUS_SUCCESS;
    }

    SpinCount = CriticalSection->SpinCount;
    if (SpinCount != 0)
    {
        do
        {

            if (CriticalSection->LockCount == -1)
            {
                if (InterlockedCompareExchange(&CriticalSection->LockCount, 0, -1) == -1)
                {
                    CriticalSection->OwningThread = Thread;
                    CriticalSection->RecursionCount = 1;
                    return STATUS_SUCCESS;
                }
            }
            else if (CriticalSection->LockCount > 0)
            {
                break;
            }

            SpinCount -= 1;
        } while (SpinCount != 0);
    }

    if (InterlockedIncrement(&CriticalSection->LockCount) != 0)
    {
       // RtlpWaitForCriticalSection(CriticalSection);
    }

    CriticalSection->OwningThread = Thread;
    CriticalSection->RecursionCount = 1;
    return STATUS_SUCCESS;
}

NTSTATUS
RtlpLeaveCriticalSection(IN PRTL_CRITICAL_SECTION CriticalSection)

/*++

Routine Description:

    This function leaves a critical section.

Arguments:

    CriticalSection - Supplies a pointer to a critical section.

Return Value:

   STATUS_SUCCESS is returned.

--*/

{
    ASSERT(NtCurrentTeb()->ClientId.UniqueThread == CriticalSection->OwningThread);

    if ((CriticalSection->RecursionCount -= 1) == 0)
    {
        CriticalSection->OwningThread = NULL;
        if (InterlockedDecrement(&CriticalSection->LockCount) >= 0)
        {
          //  RtlpUnWaitCriticalSection(CriticalSection);
        }
    }

    return STATUS_SUCCESS;
}

BOOLEAN
RtlpTryEnterCriticalSection(IN PRTL_CRITICAL_SECTION CriticalSection)

/*++

Routine Description:

    This function attempts to enter a critical section without blocking.

Arguments:

    CriticalSection (a0) - Supplies a pointer to a critical section.

Return Value:

    If the critical section was successfully entered, then a value of TRUE
    is returned. Otherwise, a value of FALSE is returned.

--*/

{

    HANDLE Thread;

    //
    // If the current thread owns the critical section, then increment
    // the lock count and the recursion count and return TRUE.
    //

    Thread = NtCurrentTeb()->ClientId.UniqueThread;
    if (Thread == CriticalSection->OwningThread)
    {

        ASSERT(CriticalSection->LockCount >= 0);

        InterlockedIncrement(&CriticalSection->LockCount);
        CriticalSection->RecursionCount += 1;
        return TRUE;
    }

    //
    // Attempt to enter the critical section. If the attempt is successful,
    // then set the owning thread, initialize the recursion count, and return
    // TRUE. Otherwise, return FALSE.
    //

    if (InterlockedCompareExchange(&CriticalSection->LockCount, 0, -1) == -1)
    {
        CriticalSection->OwningThread = Thread;
        CriticalSection->RecursionCount = 1;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

std::wstring ToWString(const char* str)
{
    std::wstring result;
    int len = static_cast<int>(strlen(str));
    if (auto need = MultiByteToWideChar(CP_UTF8, 0, str, len, nullptr, 0))
    {
        result.resize(static_cast<size_t>(need), L'\0');
        MultiByteToWideChar(CP_UTF8, 0, str, len, &result.at(0), static_cast<int>(result.size()));
    }
    return result;
}

std::string FromWString(const wchar_t* str)
{
    std::string result;
    int len = static_cast<int>(wcslen(str));
    if (auto need = WideCharToMultiByte(CP_UTF8, 0, str, len, nullptr, 0, nullptr, nullptr))
    {
        result.resize(static_cast<size_t>(need));
        WideCharToMultiByte(CP_UTF8, 0, str, len, &result.at(0), static_cast<int>(result.size()), nullptr, nullptr);
    }
    return result;
}

int GetStringIndexFromArray(const wchar_t* string, const wchar_t** array, int array_size)
{
    for (int i = 0; i < array_size; i++)
    {
        if (wcscmp(string, array[i]) == 0)
            return i;
    }
    return -1;
}

PIMAGE_NT_HEADERS
RtlImageNtHeader(
    IN PVOID Base
)

/*++

Routine Description:

    This function returns the address of the NT Header.

Arguments:

    Base - Supplies the base of the image.

Return Value:

    Returns the address of the NT Header.

--*/

{
    PIMAGE_NT_HEADERS NtHeaders = NULL;

    if (Base != NULL && Base != (PVOID)-1) {
        if (((PIMAGE_DOS_HEADER)Base)->e_magic == IMAGE_DOS_SIGNATURE) {
            NtHeaders = (PIMAGE_NT_HEADERS)((PCHAR)Base + ((PIMAGE_DOS_HEADER)Base)->e_lfanew);

            if (NtHeaders->Signature != IMAGE_NT_SIGNATURE) {
                NtHeaders = NULL;
            }
        }
    }

    return NtHeaders;
}

int
LdrpGenericExceptionFilter(
	IN const struct _EXCEPTION_POINTERS* ExceptionPointers,
	IN PCSTR FunctionName
)
{
	return EXCEPTION_EXECUTE_HANDLER;
}

NTSTATUS
NTAPI
LdrEnumerateLoadedModules(
	ULONG Flags,
	PLDR_LOADED_MODULE_ENUMERATION_CALLBACK_FUNCTION CallbackFunction,
	PVOID Context
)
{
	NTSTATUS Status;
	BOOLEAN LoaderLockLocked = FALSE;
	PLIST_ENTRY LoadOrderListHead = NULL;
	PLIST_ENTRY ListEntry;
	BOOLEAN StopEnumeration = FALSE;
	PVOID   LockCookie = NULL;

	if ((Flags != 0) ||
		(CallbackFunction == NULL)) {
		Status = STATUS_INVALID_PARAMETER;
		goto Exit;
	}

	Status = LdrLockLoaderLock(0, NULL, &LockCookie);
	if (!NT_SUCCESS(Status))
		goto Exit;

	LoaderLockLocked = TRUE;
	LoadOrderListHead = &NtCurrentPeb()->Ldr->InLoadOrderModuleList;

	ListEntry = LoadOrderListHead->Flink;

	while (ListEntry != LoadOrderListHead) {
		__try {
			(*CallbackFunction)(
				CONTAINING_RECORD(ListEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks),
				Context,
				&StopEnumeration);
		}
		__except (LdrpGenericExceptionFilter(GetExceptionInformation(), __FUNCTION__)) {
			Status = GetExceptionCode();
			goto Exit;
		}

		if (StopEnumeration)
			break;

		ListEntry = ListEntry->Flink;
	}

	Status = LdrUnlockLoaderLock(0, 0);
	LoaderLockLocked = FALSE;
	if (!NT_SUCCESS(Status))
		goto Exit;

	Status = STATUS_SUCCESS;
Exit:
	if (LoaderLockLocked) {
		NTSTATUS Status2 = LdrUnlockLoaderLock(0, 0);
		ASSERT(NT_SUCCESS(Status2));
	}

	return Status;
}

PVOID LdrpHeap = NULL;

NTSTATUS
LdrpAllocateUnicodeString(
    OUT PUNICODE_STRING StringOut,
    IN USHORT Length
)
/*++

Routine Description:

    This routine allocates space for a UNICODE_STRING from the loader
    private heap.

Arguments:

    StringOut - Pointer to UNICODE_STRING in which the information about
        the allocated string is written.  Any previous contents of StringOut
        are overwritten and lost.

    Length - length, in bytes, of the string which StringOut must be able
        to hold.

Return Value:

    NTSTATUS indicating success or failure of this function.  In general
    the only reasons it fails are STATUS_NO_MEMORY when the heap allocation
    cannot be performed or STATUS_INVALID_PARAMETER when an invalid parameter
    value is passed in.

--*/
{
    NTSTATUS st = STATUS_INTERNAL_ERROR; // returned if someone messes up and forgets to otherwise set it

    if (StringOut != NULL) {
        StringOut->Length = 0;
        StringOut->MaximumLength = 0;
        StringOut->Buffer = NULL;
    }

    if ((StringOut == NULL) ||
        ((Length % sizeof(WCHAR)) != 0)) {
        st = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    StringOut->Buffer = (PWSTR) RtlAllocateHeap(LdrpHeap, 0, Length + sizeof(WCHAR));
    if (StringOut->Buffer == NULL) {
        st = STATUS_NO_MEMORY;
        goto Exit;
    }

    StringOut->Buffer[Length / sizeof(WCHAR)] = L'\0';
    StringOut->Length = 0;

    // If the true length of the buffer can be represted in 16 bits, store it; otherwise
    // store the biggest number we can.
    if (Length != UNICODE_STRING_MAX_BYTES)
        StringOut->MaximumLength = Length + sizeof(WCHAR);
    else
        StringOut->MaximumLength = Length;

    st = STATUS_SUCCESS;
Exit:
    return st;
}


NTSTATUS
LdrpCopyUnicodeString(
    OUT PUNICODE_STRING StringOut,
    IN PCUNICODE_STRING StringIn
)
/*++

Routine Description:

    This function makes a copy of a unicode string; the important aspect
    of it is that the string is allocated from the loader private heap.

Arguments:

    StringOut - Pointer to UNICODE_STRING in which the information about
        the copied string is written.  Any previous contents of StringOut
        are overwritten and lost.

    StringIn - Pointer to constant UNICODE_STRING which is copied.

Return Value:

    NTSTATUS indicating success or failure of this function.  In general
    the only reason it fails is STATUS_NO_MEMORY when the heap allocation
    cannot be performed.

--*/

{
    NTSTATUS st = STATUS_INTERNAL_ERROR;
    ULONG BytesNeeded = 0;

    if (StringOut != NULL) {
        StringOut->Length = 0;
        StringOut->MaximumLength = 0;
        StringOut->Buffer = NULL;
    }

    if ((StringOut == NULL) ||
        (StringIn == NULL)) {
        st = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    st = RtlValidateUnicodeString(0, StringIn);
    if (!NT_SUCCESS(st))
        goto Exit;

    st = LdrpAllocateUnicodeString(StringOut, StringIn->Length);
    if (!NT_SUCCESS(st))
        goto Exit;

    RtlCopyMemory(StringOut->Buffer, StringIn->Buffer, StringIn->Length);
    StringOut->Length = StringIn->Length;

    st = STATUS_SUCCESS;
Exit:
    return st;
}

VOID
LdrpFreeUnicodeString(
    IN OUT PUNICODE_STRING StringIn
)
/*++

Routine Description:

    This function deallocates a string that was allocated using
    LdrpCopyUnicodeString.

Arguments:

    String - Pointer to UNICODE_STRING which is to be freed.  On exit,
                all the members are set to 0/null as appropriate.

Return Value:

    None

--*/

{
    if (StringIn != NULL) {
        if (StringIn->Buffer != NULL) {
            RtlFreeHeap(LdrpHeap, 0, StringIn->Buffer);
        }

        StringIn->Length = 0;
        StringIn->MaximumLength = 0;
        StringIn->Buffer = NULL;
    }
}

