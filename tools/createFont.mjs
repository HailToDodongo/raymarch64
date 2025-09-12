/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
import fs from 'fs';
import {PNG} from 'pngjs';
import { strict as assert } from 'node:assert';

const pngPath = process.argv[2];
const cPath = process.argv[3];

let srcData = "uint64_t FONT_8x8_DATA[] = {\n";

/**
 * Encodes a monochrome font into a bitmask array.
 * This will store a single 8x8 char as a 64bit int
 */
fs.createReadStream(pngPath)
  .pipe(new PNG())
  .on('parsed', function()
  {
    assert.equal(this.width, 8);
    assert.equal(this.height % 8, 0);

    const chars = this.height / 8;

    let idx = 0;
    for(let c=0; c<chars; ++c)
    {
      let mask = 0n;
      let shift = 0n;
      for(let p=0; p<8*8; ++p) {
        let isSet = this.data[idx] > 128;
        mask |= BigInt(isSet) << shift;
        ++shift;
        idx += 4;
      }

      srcData += `0x${mask.toString(16).padStart(16, '0')},\n`;
    }
    srcData += "\n};\n";

    fs.writeFileSync(cPath, srcData);
});