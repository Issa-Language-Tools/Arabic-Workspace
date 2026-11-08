# Arabic Workspace

A Qt-based Arabic language learning workspace (C++17, Qt6 Widgets, SQLite3).

## Ticket prefix

ticket-prefix: ARA

Maps ticket keys like `ARA-XXX` to this repository's GitHub issues
(`Issa-Language-Tools/Arabic-Workspace#XXX`). Keep this line on its own and
unchanged — the ticket-lookup tooling depends on it.

## Build & run

- Build: `make all`
- Build & run: `make run`
- Run without suppressing Qt logging: `make run-debug`
- Clean: `make clean`

Binary output: `build/release/workspace`.

## Layout

- `src/` — application logic & UI source
- `structs/` — global custom data types
- `migrations/` — SQL migration files
- `scripts/` — developer utility scripts (e.g. migration generator)
- `build/` — generated build output (not committed)
- `words.db` — SQLite database created on first run

## Migrations

Create a new migration with:

```bash
scripts/migrate <name>
```

This creates a file in `migrations/` with the proper naming convention.
