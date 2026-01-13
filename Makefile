all: build

.PHONY: build
build:
	pio run

.PHONY: run
run:
	pio run --target upload

.PHONY: monitor
monitor:
	pio device monitor

.PHONY: clean
clean:
	pio run --target clean

# Creates compile_commands.json
.PHONY: clangd
clangd:
	./scripts/generate.clangd.py
