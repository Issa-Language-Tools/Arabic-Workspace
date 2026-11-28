import express from "express";
import cors from "cors";

import { searchLughatana } from "./dictionaries/lughatunaSearch.ts";
import { searchWiktionary } from "./dictionaries/wiktionarySearch.ts";

const app = express();
const PORT = Number(process.env.PORT ?? 8420);

app.use(cors());
app.use(express.json());
app.set("json spaces", 2);

app.get("/health", (_req, res) => {
  res.status(200).json({ ok: true });
});

app.post("/lookup", async (req, res) => {
  try {
    const { word } = req.body;

    if (!word || typeof word !== "string") {
      throw new Error("invalid word");
    }

    const results = [await searchWiktionary(word), await searchLughatana(word)];

    res.status(200).json(results);
  } catch (error: any) {
    console.log("Server error in /lookup:", error.message);
    res.status(400).json({ error: error.message });
  }
});

app.listen(PORT, "127.0.0.1", () => {
  console.log(`dictionary service listening on http://127.0.0.1:${PORT}`);
});
