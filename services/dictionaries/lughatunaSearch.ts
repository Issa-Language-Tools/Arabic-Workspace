import * as cheerio from "cheerio";

import type { Dictionary, Root, SearchResult, Word } from "../types.ts";

// search options for fus7a and levantine arabic without examples
const ISSA_PREFERENCES =
  "&dc%5B%5D=1&dc%5B%5D=8&st%5B%5D=0&st%5B%5D=1&st%5B%5D=2";

export async function searchLughatana(
  searchTerm: string,
): Promise<SearchResult> {
  console.log("searchLughatana called");

  const cleanTerm = searchTerm.trim();
  const encodedTerm = encodeURIComponent(cleanTerm);

  console.log("clean term ", cleanTerm, " encodedTerm ", encodedTerm);
  //ensure searchTerm is not encoded
  if (decodeURI(searchTerm) !== searchTerm)
    throw new Error("Input Error: string must be in raw form");

  const response = await fetch(
    `https://www.livingarabic.com/en/search?q=${encodedTerm}${ISSA_PREFERENCES}`,
    {
      headers: {
        "User-Agent":
          "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/124.0.0.0 Safari/537.36",
        Accept:
          "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8",
        "Accept-Language": "en-US,en;q=0.9",
        Connection: "keep-alive",
      },
    },
  );
  const html = await response.text();

  const entry = parseLughatana(html);

  return entry;
}

function parseLughatana(html: string): SearchResult {
  const $ = cheerio.load(html);
  const results: SearchResult = {
    dictionaries: [],
    stats: {
      totalDictionaries: 0,
      totalRoots: 0,
      totalWords: 0,
    },
  };

  // Find all dictionary sections
  $(".dictionary-title").each((_dictIndex, dictElement) => {
    const $dictTitle = $(dictElement);

    // Extract dictionary name and count
    const dictName = $dictTitle
      .find("span")
      .text()
      .replace(/\(.*\)/g, "")
      .trim();

    // Get the dictionary content section (next collapse element)
    const href = $dictTitle.find("a").attr("href");
    if (!href) return;

    const dictId = href.replace("#", "");
    const $dictContent = $(`#${dictId} .search-result-content`);

    const dictionary: Dictionary = {
      name: dictName,
      roots: [],
    };

    // Extract all roots for this dictionary
    $dictContent.find(".root").each((_rootIndex, rootElement) => {
      const $root = $(rootElement);

      // Extract root
      const root = $root.find('td:contains("Root:") + td').text().trim();

      // Extract words for this root
      const words: Word[] = [];

      $root.find(".root-children table").each((_i, table) => {
        const $table = $(table);

        // Get the word from the second <td> in the header row
        const word = $table.find("thead tr td:nth-child(2)").text().trim();

        if (word) {
          const meaning = $table
            .find(".meaning")
            .text()
            .replace(/\[pronunciation: [^\]]*\]/g, "")
            .replace(/\[keyword\]/g, "")
            .replace(/\s+/g, " ")
            .trim();

          // Extract examples
          const examples: string[] = [];
          $table.find(".examples-details p").each((_j, p) => {
            const example = $(p)
              .text()
              .replace(/\[[A-Z]+\]/g, "")
              .replace(/\s+/g, " ")
              .trim();
            if (example) examples.push(example);
          });

          words.push({
            word,
            meaning,
            examples,
          });
        }
      });

      if (root && words.length > 0) {
        dictionary.roots.push({
          root,
          words,
        });
      }
    });

    if (dictionary.roots.length > 0) {
      results.dictionaries.push(dictionary);
    }
  });

  // Calculate total counts
  results.success = true;
  results.stats.totalDictionaries = results.dictionaries.length;
  results.stats.totalRoots = results.dictionaries.reduce(
    (sum, dict) => sum + dict.roots.length,
    0,
  );
  results.stats.totalWords = results.dictionaries.reduce(
    (sum, dict) =>
      sum +
      dict.roots.reduce(
        (wordSum: number, root: Root) => wordSum + root.words.length,
        0,
      ),
    0,
  );

  return results;
}
