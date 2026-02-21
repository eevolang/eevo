# Web Assembly Build
# Compile eevo interpreter to WASM with emscripten

WASMFLAGS = -s EXPORTED_FUNCTIONS='["_eevo_read_eval_print"]' \
            -s EXPORTED_RUNTIME_METHODS='["cwrap"]' \
            -s NO_EXIT_RUNTIME=1

.PHONY: wasm
wasm: wasm-options $(EXE)$(VER).h wasm/$(EXE).js

.PHONY: wasm-options
wasm-options:
	@echo $(EXE).wasm build options:
	@echo "WASMFLAGS = $(WASMFLAGS)"
	@echo "LDFLAGS   = $(LDFLAGS)"

wasm/$(EXE).js: $(EXE)$(VER).c wasm/repl.c
	@echo emcc -o $@
	@emcc -o $@ $^ $(LDFLAGS) $(WASMFLAGS)

.PHONY: clean-wasm
clean-wasm:
	rm -f wasm/$(EXE).{html,js,wasm}
