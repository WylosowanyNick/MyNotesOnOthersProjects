# [Batch Rendering](https://www.youtube.com/watch?v=Th4huqR77rI&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&index=29)

Batch Rendering w podstawowym rozumieniu to renderowanie wielu części geometrii w jednym poleceniu rysowania. Jest to wykonywane poprzez:
- umieszczenie wszystkiego co potrzeba w jednym vertex i index buforze,
- a następnie narysowanie tego wszystkiego z wykorzystaniem jednego polecenia rysowania.

W takim przypadku, wraz ze zmienianiem się geometrii, zmienia się vertex i index bufor.
