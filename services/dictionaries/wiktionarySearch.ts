import type { Dictionary, Root, SearchResult, Word } from "../types.ts";

const WIKTIONARY_API = "https://en.wiktionary.org/w/api.php";

export async function searchWiktionary(
  searchTerm: string,
): Promise<SearchResult> {
  const cleanTerm = searchTerm.trim();
  if (decodeURI(searchTerm) !== searchTerm)
    throw new Error("Input Error: string must be in raw form");

  const searchUrl = `${WIKTIONARY_API}?action=query&format=json&list=search&srnamespace=0&srlimit=10&srsearch=${encodeURIComponent(cleanTerm)}`;

  const searchResponse = await fetch(searchUrl, {
    headers: { "User-Agent": "Arabic-Workspace/1.0 (language learning app)" },
  });
  const searchData = await searchResponse.json();

  const titles = (searchData?.query?.search ?? [])
    .map((r: { title: string }) => r.title)
    .join("|");

  if (!titles) {
    return {
      dictionaries: [],
      success: true,
      stats: {
        totalDictionaries: 0,
        totalRoots: 0,
        totalWords: 0,
      },
    };
  }

  const contentUrl = `${WIKTIONARY_API}?action=query&format=json&prop=revisions&rvprop=content&rvslots=main&titles=${encodeURIComponent(titles)}`;

  const contentResponse = await fetch(contentUrl, {
    headers: { "User-Agent": "Arabic-Workspace/1.0 (language learning app)" },
  });
  const contentData = await contentResponse.json();

  const pages = Object.values(contentData?.query?.pages ?? {});

  const dictionaries = new Map<string, Dictionary>();

  for (const page of pages as Array<{
    title?: string;
    revisions?: Array<{ slots?: { main?: { "*"?: string } } }>;
  }>) {
    const title = page.title ?? "";
    const wikitext = page.revisions?.[0]?.slots?.main?.["*"] ?? "";

    for (const entry of parseWiktionary(wikitext, title)) {
      let dictionary = dictionaries.get(entry.name);
      if (!dictionary) {
        dictionary = { name: entry.name, roots: [] };
        dictionaries.set(entry.name, dictionary);
      }
      dictionary.roots.push(entry.root);
    }
  }

  const results: SearchResult = {
    dictionaries: [],
    stats: {
      totalDictionaries: 0,
      totalRoots: 0,
      totalWords: 0,
    },
  };

  for (const dictionary of dictionaries.values()) {
    results.dictionaries.push(dictionary);
  }

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

function parseWiktionary(
  wikitext: string,
  title: string,
): Array<{ name: string; root: Root }> {
  const headers = [
    { header: "==South Levantine Arabic==", name: "Wiktionary — South Levantine Arabic" },
    { header: "==Arabic==", name: "Wiktionary — Arabic" },
  ];

  const entries: Array<{ name: string; root: Root }> = [];

  for (const { header, name } of headers) {
    const start = wikitext.indexOf(header);
    if (start === -1) continue;

    const rest = wikitext.slice(start + header.length);
    const nextHeader = rest.search(/^==[^=]/m);
    const section = nextHeader === -1 ? rest : rest.slice(0, nextHeader);
    const lines = section.split("\n").map((line) => line.trim());

    const words = parseSection(lines, title);
    if (words.length > 0) {
      entries.push({ name, root: { root: title, words } });
    }
  }

  return entries;
}

function parseSection(lines: string[], title: string): Word[] {
  const clean = (text: string): string =>
    text
      .replace(/{{[^}]*}}/g, "")
      .replace(/\[\[([^\]|]*\|)?([^\]]*)\]\]/g, "$2")
      .replace(/''|'''/g, "")
      .replace(/\s+/g, " ")
      .trim();

  const words: Word[] = [];
  let currentExamples: string[] = [];

  const isMainDef = (line: string) =>
    line.startsWith("#") &&
    !line.startsWith("#:") &&
    !line.startsWith("##") &&
    !line.startsWith("#*");

  const pushWord = () => {
    if (currentExamples.length > 0) {
      words.push({
        word: title,
        meaning: currentExamples[0],
        examples: currentExamples.slice(1),
      });
      currentExamples = [];
    }
  };

  for (const line of lines) {
    if (isMainDef(line)) {
      pushWord();
      currentExamples = [clean(line.replace(/^#+\s*/, ""))];
    } else if (line.startsWith("#*")) {
      const quote = clean(line.replace(/^#\*\s*/, ""));
      if (quote && currentExamples.length > 0) currentExamples.push(quote);
    } else if (line.startsWith("#:") && currentExamples.length > 0) {
      const gloss = clean(line.replace(/^#:\s*/, ""));
      if (gloss) currentExamples.push(gloss);
    }
  }
  pushWord();

  return words;
}
