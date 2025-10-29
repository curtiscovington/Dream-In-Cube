#!/usr/bin/env node
import { promises as fs } from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);
const PROJECT_ROOT = path.resolve(__dirname, '..');
const LEVELS_DIR = path.join(PROJECT_ROOT, 'levels');
const OUTPUT_PATH = path.join(PROJECT_ROOT, 'assets', 'levels.json');

const FACE_ORDER = ['front', 'top', 'left', 'right', 'bottom', 'back'];

async function main() {
  const entries = await fs.readdir(LEVELS_DIR, { withFileTypes: true });
  const levelDirs = entries
    .filter((entry) => entry.isDirectory() && entry.name.startsWith('level_') && entry.name !== 'level_(TEMPLATE)')
    .map((entry) => entry.name)
    .sort((a, b) => {
      const numA = Number.parseInt(a.split('_')[1], 10);
      const numB = Number.parseInt(b.split('_')[1], 10);
      return numA - numB;
    });

  const levels = [];

  for (const dirName of levelDirs) {
    const levelId = Number.parseInt(dirName.split('_')[1], 10);
    const faces = {};

    for (const face of FACE_ORDER) {
      const filePath = path.join(LEVELS_DIR, dirName, `${face}.txt`);
      const raw = await fs.readFile(filePath, 'utf8');
      const lines = raw
        .replace(/\r/g, '')
        .split('\n')
        .filter((line, idx, arr) => !(line === '' && idx === arr.length - 1))
        .map((line) => (line.length < 10 ? line.padEnd(10, ' ') : line.slice(0, 10)));

      if (lines.length !== 10) {
        throw new Error(`Level ${levelId} face ${face} has ${lines.length} lines (expected 10).`);
      }

      faces[face] = lines;
    }

    levels.push({ id: levelId, faces });
  }

  await fs.writeFile(OUTPUT_PATH, JSON.stringify({ levels }, null, 2));
  console.log(`Wrote ${levels.length} levels to ${path.relative(PROJECT_ROOT, OUTPUT_PATH)}`);
}

main().catch((error) => {
  console.error(error);
  process.exitCode = 1;
});
