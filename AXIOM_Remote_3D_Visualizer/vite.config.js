import { defineConfig } from 'vite';
import path from 'path';

export default defineConfig({
  // Vite expects index.html at the root by default
  root: './',
  base: './',
  publicDir: 'src',
  resolve: {
    alias: {
      // Direct fix for Emscripten's Node.js environment detection
      'node:fs': path.resolve(__dirname, 'empty.js'),
      'node:crypto': path.resolve(__dirname, 'empty.js'),
      'fs': path.resolve(__dirname, 'empty.js'),
      'crypto': path.resolve(__dirname, 'empty.js'),
      'path': path.resolve(__dirname, 'empty.js'),
    },
  },
  server: {
    port: 8080,
    open: true,
  },
});