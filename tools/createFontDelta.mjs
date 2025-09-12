/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
import fs from 'fs';
import {PNG} from 'pngjs';
import { strict as assert } from 'node:assert';

const pngPath = process.argv[2];
const cPath = process.argv[3];

let srcData = "uint8_t FONT_64_DATA[] = {\n";
let srcIdx = "uint32_t FONT_64_IDX[] = {\n";

/**
 * Encodes a monochrome font into a bitmask array.
 * This will store a single 8x8 char as a 64bit int
 */
fs.createReadStream(pngPath)
  .pipe(new PNG())
  .on('parsed', function()
  {
    assert.equal(this.width, 64);
    assert.equal(this.height % 64, 0);

    const chars = this.height / 64;

    let dataOffset = 0;
    let idx = 0;
    for(let c=0; c<chars; ++c)
    {
      assert.equal(this.data[idx], 0); // must start on black
      srcIdx += dataOffset + ",";

      for(let y=0; y<64; ++y) {
        let count = 0;
        let lastSet = false;

        for(let x=0; x<64; ++x)
        {
          let isSet = this.data[idx] > 128;
          if(lastSet !== isSet) {
            lastSet = isSet;
            srcData += count+',';
            ++dataOffset;
            count = 0;
          }
          ++count;
          idx += 4;
        }

        if(count) {
          srcData += count+',';
          ++dataOffset;
        }
      }

      srcData += "\n";
    }
    srcData += "\n};\n";
    srcIdx +=  "\n};\n";

    fs.writeFileSync(cPath, srcData + "\n" + srcIdx);
});