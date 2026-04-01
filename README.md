# Cereon Template Framework

Cereon Template Framework is C++ 23 standard template library for The Aridity Team C++ projects to use,
it is currently used by the work-in-progress Cereon project.

## Features

The current features of CTF (which are usually called frameworks by us) are grouped into two tiers,
based on the kind of dependencies that they have.

> [!NOTE]
> Some of the features may not work as you expected and some of the code base are badly written
> since I haven't use them in the Cereon project currently 

### Tier 0

Tier 0 frameworks don't depend on anything other than the C/C++ standard template libraries.

- Basic and common collection utilites
- Basic and common string utilities
- Basic and common memory utilities
- Basic and common threading utilities

### Tier 1 Framework

Tier 1 frameworks contains general-purpose implementations that depend on the Tier 0 frameworks for their functionality but they are still considered low-level.

- Basic logging system
- Command-line interface parser (argument parser)
- File system utilities
- I/O Stream utilities

### Tier 2 Framework

Tier 2 frameworks contains high-level general-purpose implementations like format-specific parsers and depends on both the Tier 0 and Tier 1 frameworks for their functionality.

- JSON parser
- Valve KeyValues parser

----

The following will be listed as a tree depending on which namespace is the API on, global functions or variables are excluded:

- CTF
	- Collections
		- Hash<T>
		- List<T>
		- Map<K, V>
		- Pair<K, V>
		- Vector<T>
	- FileSystem
		- File
		- Path
	- Json
		- JsonAllocator
		- JsonArray
		- JsonObject
		- JsonObjectEntry
		- JsonParser
		- JsonType
		- JsonValue
	- KV
		- KeyValues
		- KeyValuesParser
		- KeyValuesType
		- KeyValuesValue
	- Logging
		- ConsoleLogListener
		- ILogListener
		- Logger
		- LoggerSystem
	- Threading
		- Task
		- Thread
		- ThreadImpl
		- Mutex
		- LockGuard
	- Utilities
		- CLIParser
	- AnonymousPipeStream
	- BasicString<T>
        - String
        - WString
    - Color
	- FileStream
	- IStream
	- LinearArena
	- Memory
	- NamedPipeStream
	- OStream
	- OStringStream
    - PipeStream
	- RefCounted
	- RefCounter
	- RefPtr
	- Stream
