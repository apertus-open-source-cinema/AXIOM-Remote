/**
 * Firmware module wrapper
 * Wraps the emscripten WebAssembly module and exports enums and classes
 */

// Import the compiled firmware module
import axiomRemoteFirmware from "./FW/axiom_remote_firmware.mjs";

// Async function to load the firmware module
export async function loadFirmware(options = {}) {
  const module = await axiomRemoteFirmware({
    locateFile(path) {
      if (path.endsWith(".wasm")) {
        // Using a leading slash ensures the path is relative to the web root,
        // which is safer when using Vite's dev server or nested routes.
        return "/src/js/FW/" + path;
      }
      return path;
    },
    ...options
  });

  return module;
}
