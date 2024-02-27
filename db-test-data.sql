INSERT INTO prod.Specie (nomeLatino, colore, nomeComune, giacenza, prezzo, esotica, giardAppart) VALUES
    ('Rosam', 'Rosso', 'Rosa', 100, 1200, FALSE, 'giardino'),
    ('Rosam', 'Bianco', 'Rosa', 100, 1400, FALSE, 'giardino'),
    ('Rosam', 'Rosa', 'Rosa', 100, 1300, FALSE, 'giardino'),
    ('Fiordalisom', 'Viola', 'Fiordaliso', 100, 100, FALSE, 'giardino'),
    ('Orchideam', 'Rosa', 'Orchidea', 100, 2100, TRUE, 'appartamento'),
    ('Orchideam', 'Viola', 'Orchidea', 100, 2300, TRUE, 'appartamento'),
    ('Aloem', '', 'Aloe vera', 100, 150, FALSE, 'appartamento'),
    ('Rosmarinom', '', 'Rosmarino', 100, 150, FALSE, 'appartamento'),
    ('Basilicom', '', 'Basilico', 100, 150, FALSE, 'appartamento');

INSERT INTO prod.Rivendita (pIVA, nomeRivendita, indSpedizione, indFatturazione, nomeRef, cognomeRef) VALUES
    ('123', 'Leroi Merlino', 'via le mani dal naso 11', 'via le mani dal naso 11', 'Mario', 'Gialli'),
    ('345', 'ICHEA', 'via di san marino 30', 'via di san marino 30', 'Marino', 'Blu');

INSERT INTO prod.Fornitore (idFornitore, nomeFornitore, codFisc) VALUES
    (1, 'Giallo Flowers', 'qwerty'),
    (2, 'Franco', 'ammiocuggino99'),
    (3, 'Falchi Falcao', 'asr');

INSERT INTO prod.Fornisce (idFornitore, nomeLatino, colore) VALUES
    (1, 'Aloem', ''),
    (1, 'Rosmarinom', ''),
    (1, 'Basilicom', ''),
    (2, 'Aloem', ''),
    (2, 'Rosmarinom', ''),
    (2, 'Fiordalisom', 'Viola'),
    (3, 'Rosam', 'Rosso'),
    (3, 'Rosam', 'Bianco'),
    (3, 'Rosam', 'Rosa'),
    (3, 'Fiordalisom', 'Viola'),
    (3, 'Orchideam', 'Rosa'),
    (3, 'Orchideam', 'Viola');

