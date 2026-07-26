const JSZip = require('jszip');
const fs = require('fs');
const path = require('path');
const { parseString } = require('xml2js');

// Check if xml2js is available, if not use a simple parser
let parseXml;
try {
  const xml2js = require('xml2js');
  parseXml = (xml) => new Promise((resolve, reject) => {
    xml2js.parseString(xml, { explicitArray: false, ignoreAttrs: false, mergeAttrs: false }, (err, result) => {
      if (err) reject(err);
      else resolve(result);
    });
  });
} catch {
  // Simple fallback parser
  parseXml = async (xml) => {
    const { XMLParser } = require('fast-xml-parser');
    const parser = new XMLParser({ ignoreAttributes: false, attributeNamePrefix: '@_' });
    return parser.parse(xml);
  };
}

const NS = {
  w: 'http://schemas.openxmlformats.org/wordprocessingml/2006/main',
  r: 'http://schemas.openxmlformats.org/officeDocument/2006/relationships',
  wp: 'http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing',
  a: 'http://schemas.openxmlformats.org/drawingml/2006/main',
  pic: 'http://schemas.openxmlformats.org/drawingml/2006/picture',
  mc: 'http://schemas.openxmlformats.org/markup-compatibility/2006',
  wps: 'http://schemas.microsoft.com/office/word/2010/wordprocessingShape',
  v: 'urn:schemas-microsoft-com:vml',
  wpc: 'http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas',
  mo: 'http://schemas.microsoft.com/office/mac/office/2008/main',
  mcotyp: 'http://schemas.openxmlformats.org/package/2006/content-types',
  w14: 'http://schemas.microsoft.com/office/word/2010/wordml',
};

function stripNs(tag) {
  return tag.includes(':') ? tag.split(':')[1] : tag;
}

function simplifyAttrs(obj) {
  if (!obj || typeof obj !== 'object') return obj;
  if (Array.isArray(obj)) return obj.map(simplifyAttrs);
  const result = {};
  for (const [k, v] of Object.entries(obj)) {
    const key = stripNs(k).replace(/^@_/, '');
    if (k === '$' || key === '$') {
      // Merge attributes
      if (typeof v === 'object') {
        for (const [ak, av] of Object.entries(v)) {
          result[stripNs(ak)] = av;
        }
      }
    } else if (key.startsWith('@_') || key.startsWith('@')) {
      result[key.replace(/^@_?/, '')] = v;
    } else if (!Array.isArray(v) && typeof v === 'object' && v !== null) {
      result[key] = simplifyAttrs(v);
    } else if (Array.isArray(v)) {
      result[key] = v.map(simplifyAttrs);
    } else {
      result[key] = v;
    }
  }
  return result;
}

function getVal(obj, ...keys) {
  if (!obj) return undefined;
  for (const key of keys) {
    if (obj[key] !== undefined) return obj[key];
  }
  // Check val attribute
  if (obj['@_w:val'] !== undefined) return obj['@_w:val'];
  if (obj['@_val'] !== undefined) return obj['@_val'];
  if (obj.val !== undefined) return obj.val;
  return undefined;
}

async function main() {
  const docxPath = 'D:\\new-begin\\Project3\\04课程设计报告-王仕鹏.docx';
  const data = fs.readFileSync(docxPath);
  const zip = await JSZip.loadAsync(data);

  // List all files in the zip
  console.log('========================================');
  console.log('DOCX FILE CONTENTS');
  console.log('========================================');
  const files = Object.keys(zip.files).sort();
  for (const f of files) {
    const entry = zip.files[f];
    const size = entry._data ? (entry._data.compressedSize || 0) : 0;
    console.log(`  ${f} (${entry.dir ? 'DIR' : size + ' bytes'})`);
  }
  console.log('');

  // Helper to read and parse XML
  async function readXml(filePath) {
    if (!zip.files[filePath]) {
      return null;
    }
    const content = await zip.files[filePath].async('text');
    return parseXml(content);
  }

  // 1. Document defaults from styles.xml
  console.log('========================================');
  console.log('1. DOCUMENT DEFAULTS (docDefaults)');
  console.log('========================================');
  const stylesXml = await readXml('word/styles.xml');
  if (stylesXml) {
    const styles = stylesXml['w:styles'] || stylesXml.styles || stylesXml;
    const docDefaults = styles['w:docDefaults'] || styles.docDefaults;
    if (docDefaults) {
      const dd = simplifyAttrs(docDefaults);
      console.log(JSON.stringify(dd, null, 2));
    } else {
      console.log('No docDefaults found');
    }
  }
  console.log('');

  // 2. All styles
  console.log('========================================');
  console.log('2. ALL STYLES');
  console.log('========================================');
  if (stylesXml) {
    const styles = stylesXml['w:styles'] || stylesXml.styles || stylesXml;
    let styleArray = styles['w:style'] || styles.style || [];
    if (!Array.isArray(styleArray)) styleArray = [styleArray];
    for (const style of styleArray) {
      const s = simplifyAttrs(style);
      const styleId = s['@_w:styleId'] || s['@_styleId'] || s.styleId;
      const type = s['@_w:type'] || s['@_type'] || s.type;
      const name = (s.name && (s.name['@_w:val'] || s.name.val)) || '';
      console.log(`\n--- Style: ${name} (ID: ${styleId}, Type: ${type}) ---`);
      if (s.pPr) {
        console.log('  pPr:', JSON.stringify(s.pPr, null, 4).replace(/\n/g, '\n  '));
      }
      if (s.rPr) {
        console.log('  rPr:', JSON.stringify(s.rPr, null, 4).replace(/\n/g, '\n  '));
      }
      if (s.tblPr) {
        console.log('  tblPr:', JSON.stringify(s.tblPr, null, 4).replace(/\n/g, '\n  '));
      }
    }
  }
  console.log('');

  // 3. Document body
  console.log('========================================');
  console.log('3. DOCUMENT BODY - ALL PARAGRAPHS');
  console.log('========================================');
  const docXml = await readXml('word/document.xml');
  if (docXml) {
    const doc = docXml['w:document'] || docXml.document || docXml;
    const body = doc['w:body'] || doc.body || doc;
    const bodyContent = body['w:p'] || body.p || [];
    const bodyTbls = body['w:tbl'] || body.tbl || [];
    const bodySectPr = body['w:sectPr'] || body.sectPr;

    const paragraphs = Array.isArray(bodyContent) ? bodyContent : [bodyContent];

    // Track tables embedded in paragraphs
    let paraIdx = 0;
    for (const para of paragraphs) {
      const p = simplifyAttrs(para);
      const pPr = p.pPr || {};
      const pStyle = pPr.pStyle ? (pPr.pStyle['@_w:val'] || pPr.pStyle.val) : null;
      const jc = pPr.jc ? (pPr.jc['@_w:val'] || pPr.jc.val) : null;
      const ind = pPr.ind || null;
      const spacing = pPr.spacing || null;

      const runs = p.r || [];
      const runArray = Array.isArray(runs) ? runs : [runs];

      console.log(`\n--- Paragraph ${paraIdx} ---`);
      if (pStyle) console.log(`  Style: ${pStyle}`);
      if (jc) console.log(`  Alignment: ${jc}`);
      if (ind) console.log(`  Indentation: ${JSON.stringify(ind)}`);
      if (spacing) console.log(`  Spacing: ${JSON.stringify(spacing)}`);
      if (pPr.numPr) console.log(`  Numbering: ${JSON.stringify(pPr.numPr)}`);
      if (pPr.shd) console.log(`  Shading: ${JSON.stringify(pPr.shd)}`);

      // Check for tables inside paragraph (word sometimes nests tbl in p)
      const tbls = p.tbl || [];
      const tblArray = Array.isArray(tbls) ? tbls : [tbls];
      for (const tbl of tblArray) {
        if (tbl && typeof tbl === 'object') {
          console.log(`  [TABLE EMBEDDED IN PARAGRAPH]`);
          const t = simplifyAttrs(tbl);
          console.log(`    Table rows: ${(t.tr || []).length}`);
        }
      }

      let textParts = [];
      for (let ri = 0; ri < runArray.length; ri++) {
        const run = runArray[ri];
        const r = simplifyAttrs(run);
        const t = r.t || r.t_text;
        const rPr = r.rPr || {};
        const text = (typeof t === 'string') ? t : '';

        const bold = rPr.b ? (rPr.b['@_w:val'] === 'true' || rPr.b['@_w:val'] === '1' || rPr.b.val === 'true' || rPr.b.val === '1' || Object.keys(rPr.b).length === 0) : false;
        const italic = rPr.i ? true : false;
        const sz = rPr.sz ? (rPr.sz['@_w:val'] || rPr.sz.val) : null;
        const szCs = rPr.szCs ? (rPr.szCs['@_w:val'] || rPr.szCs.val) : null;
        const rFonts = rPr.rFonts || null;
        const color = rPr.color ? (rPr.color['@_w:val'] || rPr.color.val) : null;
        const u = rPr.u ? (rPr.u['@_w:val'] || rPr.u.val) : null;
        const strike = rPr.strike ? true : false;
        const vertAlign = rPr.vertAlign ? (rPr.vertAlign['@_w:val'] || rPr.vertAlign.val) : null;

        if (text) {
          textParts.push(text);
          console.log(`  Run ${ri}: "${text}"`);
          if (bold) console.log(`    Bold: true`);
          if (italic) console.log(`    Italic: true`);
          if (sz) console.log(`    FontSize: ${(parseInt(sz) / 2)}pt (val=${sz})`);
          if (szCs) console.log(`    FontSizeComplex: ${(parseInt(szCs) / 2)}pt`);
          if (rFonts) console.log(`    Fonts: ${JSON.stringify(rFonts)}`);
          if (color) console.log(`    Color: ${color}`);
          if (u) console.log(`    Underline: ${u}`);
          if (strike) console.log(`    Strikethrough: true`);
          if (vertAlign) console.log(`    VerticalAlign: ${vertAlign}`);
        } else {
          // Check for drawings/images
          const drawing = r.drawing || r.wps || r.pic || run['w:drawing'] || run['w:pict'];
          if (drawing || run['w:drawing'] || run['w:pict']) {
            console.log(`  Run ${ri}: [IMAGE/DRAWING]`);
            const d = drawing || run['w:drawing'] || run['w:pict'];
            console.log(`    ${JSON.stringify(simplifyAttrs(d)).substring(0, 500)}`);
          } else if (r.br) {
            console.log(`  Run ${ri}: [LINE BREAK]`);
          } else if (r.tab) {
            console.log(`  Run ${ri}: [TAB]`);
          } else {
            const keys = Object.keys(run).filter(k => !k.startsWith('$'));
            if (keys.length > 0) {
              // Might be non-text content
              for (const key of keys) {
                const shortKey = stripNs(key);
                if (shortKey !== 'rPr' && shortKey !== 't' && shortKey !== 't_text') {
                  console.log(`  Run ${ri}: [${shortKey}]`);
                }
              }
            }
          }
        }
      }

      if (textParts.length > 0) {
        console.log(`  Full text: "${textParts.join('')}"`);
      }

      // Check for hyperlinks
      const hyperlinks = p.hyperlink || [];
      const hlArray = Array.isArray(hyperlinks) ? hyperlinks : [hyperlinks];
      for (const hl of hlArray) {
        if (hl && typeof hl === 'object') {
          const h = simplifyAttrs(hl);
          console.log(`  Hyperlink: ${JSON.stringify(h)}`);
        }
      }

      // Check for bookmarks
      const bookmarks = p.bookmarkStart || [];
      const bmArray = Array.isArray(bookmarks) ? bookmarks : [bookmarks];
      for (const bm of bmArray) {
        if (bm && typeof bm === 'object') {
          const b = simplifyAttrs(bm);
          console.log(`  Bookmark: ${JSON.stringify(b)}`);
        }
      }

      paraIdx++;
    }

    // Print standalone tables
    const tables = Array.isArray(bodyTbls) ? bodyTbls : [bodyTbls];
    for (let ti = 0; ti < tables.length; ti++) {
      const tbl = tables[ti];
      if (tbl && typeof tbl === 'object') {
        console.log(`\n--- Standalone Table ${ti} ---`);
        const t = simplifyAttrs(tbl);
        const rows = t.tr || [];
        const rowArray = Array.isArray(rows) ? rows : [rows];
        for (let ri = 0; ri < rowArray.length; ri++) {
          const row = rowArray[ri];
          const cells = row.tc || [];
          const cellArray = Array.isArray(cells) ? cells : [cells];
          const cellTexts = [];
          for (let ci = 0; ci < cellArray.length; ci++) {
            const cell = cellArray[ci];
            // Extract text from cell paragraphs
            const cellPs = cell.p || [];
            const cellPArray = Array.isArray(cellPs) ? cellPs : [cellPs];
            const cellText = cellPArray.map(cp => {
              const cpSimple = simplifyAttrs(cp);
              const runs = cpSimple.r || [];
              const runArr = Array.isArray(runs) ? runs : [runs];
              return runArr.map(rr => rr.t || '').join('');
            }).join(' | ');
            cellTexts.push(cellText);
          }
          console.log(`  Row ${ri}: ${cellTexts.join(' | ')}`);
        }
        // Table properties
        if (t.tblPr) {
          console.log(`  Table properties: ${JSON.stringify(t.tblPr)}`);
        }
      }
    }

    if (bodySectPr) {
      console.log(`\n--- Section Properties ---`);
      const sp = simplifyAttrs(bodySectPr);
      console.log(`  ${JSON.stringify(sp, null, 4)}`);
    }
  }

  // 4. Headers and Footers
  console.log('\n========================================');
  console.log('4. HEADERS AND FOOTERS');
  console.log('========================================');

  // Find relationships to headers/footers
  const relsXml = await readXml('word/_rels/document.xml.rels');
  if (relsXml) {
    const rels = relsXml['Relationships'] || relsXml;
    let relArray = rels['Relationship'] || [];
    if (!Array.isArray(relArray)) relArray = [relArray];

    for (const rel of relArray) {
      const r = simplifyAttrs(rel);
      const type = r['@_Type'] || r['@_type'] || r.Type;
      const target = r['@_Target'] || r['@_target'] || r.Target;

      if (type && type.includes('header')) {
        console.log(`\n--- Header: ${target} ---`);
        const headerXml = await readXml(path.posix.join('word', target));
        if (headerXml) {
          const header = headerXml['w:hdr'] || headerXml.hdr || headerXml;
          const hPars = header['w:p'] || header.p || [];
          const hParaArray = Array.isArray(hPars) ? hPars : [hPars];
          for (let hi = 0; hi < hParaArray.length; hi++) {
            const hp = simplifyAttrs(hParaArray[hi]);
            const runs = hp.r || [];
            const runArr = Array.isArray(runs) ? runs : [runs];
            const text = runArr.map(r => r.t || '').join('');
            console.log(`  Paragraph ${hi}: "${text}"`);
            if (hp.pPr && hp.pPr.pStyle) {
              console.log(`    Style: ${hp.pPr.pStyle['@_w:val'] || hp.pPr.pStyle.val}`);
            }
          }
        }
      }

      if (type && type.includes('footer')) {
        console.log(`\n--- Footer: ${target} ---`);
        const footerXml = await readXml(path.posix.join('word', target));
        if (footerXml) {
          const footer = footerXml['w:ftr'] || footerXml.ftr || footerXml;
          const fPars = footer['w:p'] || footer.p || [];
          const fParaArray = Array.isArray(fPars) ? fPars : [fPars];
          for (let fi = 0; fi < fParaArray.length; fi++) {
            const fp = simplifyAttrs(fParaArray[fi]);
            const runs = fp.r || [];
            const runArr = Array.isArray(runs) ? runs : [runs];
            const text = runArr.map(r => r.t || '').join('');
            console.log(`  Paragraph ${fi}: "${text}"`);
          }
        }
      }
    }
  }

  // 5. Images / Media
  console.log('\n========================================');
  console.log('5. MEDIA FILES (Images etc)');
  console.log('========================================');
  const mediaFiles = Object.keys(zip.files).filter(f => f.startsWith('word/media/'));
  for (const mf of mediaFiles) {
    const ext = path.extname(mf).toLowerCase();
    const size = (await zip.files[mf].async('nodebuffer')).length;
    console.log(`  ${mf} (${(size / 1024).toFixed(1)} KB, ${ext})`);
  }

  // 6. Numbering
  console.log('\n========================================');
  console.log('6. NUMBERING DEFINITIONS');
  console.log('========================================');
  const numXml = await readXml('word/numbering.xml');
  if (numXml) {
    const num = numXml['w:numbering'] || numXml.numbering || numXml;
    const numFmts = num['w:numFmt'] || num.numFmt || [];
    const numArr = Array.isArray(numFmts) ? numFmts : [numFmts];
    for (const nf of numArr) {
      console.log(`  ${JSON.stringify(simplifyAttrs(nf))}`);
    }
    const abstracts = num['w:abstractNum'] || num.abstractNum || [];
    const absArr = Array.isArray(abstracts) ? abstracts : [abstracts];
    for (const ab of absArr) {
      const a = simplifyAttrs(ab);
      console.log(`  abstractNum ${a['@_w:abstractNumId'] || a.abstractNumId}:`);
      const lvls = a.lvl || [];
      const lvlArr = Array.isArray(lvls) ? lvls : [lvls];
      for (const lvl of lvlArr) {
        console.log(`    level ${lvl['@_w:ilvl'] || lvl.ilvl}: ${JSON.stringify(lvl)}`);
      }
    }
    const nums = num['w:num'] || num.num || [];
    const numList = Array.isArray(nums) ? nums : [nums];
    for (const n of numList) {
      console.log(`  ${JSON.stringify(simplifyAttrs(n))}`);
    }
  } else {
    console.log('  No numbering.xml found');
  }

  // 7. Font table
  console.log('\n========================================');
  console.log('7. FONT TABLE');
  console.log('========================================');
  const fontXml = await readXml('word/fontTable.xml');
  if (fontXml) {
    const ft = fontXml['w:fonts'] || fontXml.fonts || fontXml;
    const fonts = ft['w:font'] || ft.font || [];
    const fontArr = Array.isArray(fonts) ? fonts : [fonts];
    for (const f of fontArr) {
      const ff = simplifyAttrs(f);
      console.log(`  ${JSON.stringify(ff)}`);
    }
  } else {
    console.log('  No fontTable.xml found');
  }

  // 8. Settings
  console.log('\n========================================');
  console.log('8. DOCUMENT SETTINGS');
  console.log('========================================');
  const settingsXml = await readXml('word/settings.xml');
  if (settingsXml) {
    const s = simplifyAttrs(settingsXml['w:settings'] || settingsXml.settings || settingsXml);
    console.log(`  ${JSON.stringify(s, null, 2).substring(0, 2000)}`);
  }

  // 9. Themes
  console.log('\n========================================');
  console.log('9. THEMES');
  console.log('========================================');
  const themeFiles = Object.keys(zip.files).filter(f => f.startsWith('word/theme/'));
  for (const tf of themeFiles) {
    console.log(`  Found: ${tf}`);
  }

  console.log('\n========================================');
  console.log('EXTRACTION COMPLETE');
  console.log('========================================');
}

main().catch(err => {
  console.error('Error:', err);
  process.exit(1);
});
