export interface Word {
  word: string;
  meaning: string;
  examples: string[];
}

export interface Root {
  root: string;
  words: Word[];
}

export interface Dictionary {
  name: string;
  roots: Root[];
}

export interface SearchResult {
  dictionaries: Dictionary[];
  success?: boolean;
  stats: {
    totalDictionaries: number;
    totalRoots: number;
    totalWords: number;
  };
}
