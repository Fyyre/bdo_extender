# bdo_extender

Archival snapshot of a BDO Client Extender by Fyyre

## Layout

- `bns-nogg/` — `d3d9` proxy DLL (`bdo_extender.sln`)
- `Shared/` — shared headers, hooker, LZ4, ntdll
- `BDO/` — static lib used by Shared
- `bdo.props` — common MSBuild settings

Open `bns-nogg/bdo_extender.sln`.
