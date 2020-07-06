# Zahřívací úloha

V této úloze si vyzkoušíte základní funkce programovacího jazyka.

## Problém

Valve má problém... V CS:GO došlo po leaknutí source kódu k velkému nárustu cheatrů.

A support team je přehlcen množícími tickety. Pro zrychlení komunikace je potřeba vytvořit automatický systém.

## Program

Podpůrný program bude dělat následující:

Na vstupu programu bude číselná hodnota 1-3. Tato hodnota představuje kategorii problému. Program poté podle kategorie vypíše automatickou odpověď uživateli (viz ukázka běhu) a ukončí se s návratovým kódem 0.

V případě že na vstupu programu bude neplatný vstup (prázdný vstup, neznáme číslo, jiný znak...) vypíše program chybové hlášení a ukončí se s návratových kódem 1. 

Všechny výpisy programu jsou ukončeny novou řádkou. A program ignoruje bílé znaky (mezera, nový řádek...).

### Ukázky běhu

```
Kategorie dotazu?
1
Děkujeme, uživatele právě prověřujeme a podnikneme nutné kroky.
```

```
Kategorie dotazu?
2
Děkujeme, předáme Váš feedback našemu vývojovému oddělení.
```

```    
Kategorie dotazu?
3
Vaši zprávu evidujeme a ozveme se Vám co nejdříve.
```

```
Kategorie dotazu?
5
ERROR
```

```
Kategorie dotazu?
aaaa
ERROR
```
