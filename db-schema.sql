

-- CANCELLAMIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
DROP DATABASE prod;

CREATE DATABASE IF NOT EXISTS prod;
USE prod;


CREATE TABLE IF NOT EXISTS prod.Specie(
    nomeLatino CHAR(100) NOT NULL,
    colore CHAR(30) NOT NULL,
    nomeComune VARCHAR(50) NOT NULL,
    giacenza INT NOT NULL DEFAULT 0,
    prezzo MEDIUMINT UNSIGNED NOT NULL DEFAULT 0,
    esotica BOOLEAN NOT NULL,
    giardAppart ENUM('giardino', 'appartamento') NOT NULL,
    PRIMARY KEY(nomeLatino, colore)
);


CREATE TABLE IF NOT EXISTS prod.Rivendita(
    pIVA CHAR(15) NOT NULL,
    nomeRivendita VARCHAR(50) NOT NULL,
    indSpedizione VARCHAR(50) NOT NULL,
    indFatturazione VARCHAR(50) NOT NULL,
    nomeRef VARCHAR(20) NOT NULL,
    cognomeRef VARCHAR(20) NOT NULL,
    PRIMARY KEY(pIVA)
);


CREATE TABLE IF NOT EXISTS prod.TelefonoRivendita(
    pIVA CHAR(15) NOT NULL,
    nTelefono VARCHAR(15) NOT NULL,
    PRIMARY KEY(pIVA, nTelefono),
    FOREIGN KEY(pIVA) REFERENCES Rivendita(pIVA)
);


CREATE TABLE IF NOT EXISTS prod.EmailRivendita(
    pIVA CHAR(15) NOT NULL,
    email VARCHAR(320) NOT NULL,
    PRIMARY KEY(pIVA, email),
    FOREIGN KEY(pIVA) REFERENCES Rivendita(pIVA)
);


CREATE TABLE IF NOT EXISTS prod.Fornitore(
    idFornitore MEDIUMINT UNSIGNED NOT NULL AUTO_INCREMENT,
    nomeFornitore VARCHAR(50) NOT NULL,
    codFisc VARCHAR(20) NOT NULL,
    PRIMARY KEY(idFornitore)
);


CREATE TABLE IF NOT EXISTS prod.IndirizzoFornitore(
    idFornitore MEDIUMINT UNSIGNED NOT NULL,
    indirizzo VARCHAR(50) NOT NULL,
    PRIMARY KEY(idFornitore, indirizzo),
    FOREIGN KEY(idFornitore) REFERENCES Fornitore(idFornitore)
);


CREATE TABLE IF NOT EXISTS prod.Fornisce(
    idFornitore MEDIUMINT UNSIGNED NOT NULL,
    nomeLatino CHAR(100) NOT NULL,
    colore CHAR(30) NOT NULL,
    PRIMARY KEY(idFornitore, nomeLatino, colore),
    FOREIGN KEY(idFornitore) REFERENCES Fornitore(idFornitore),
    FOREIGN KEY(nomeLatino, colore) REFERENCES Specie(nomeLatino, colore)
);


CREATE TABLE IF NOT EXISTS prod.OrdineVendita(
    idVendita INT UNSIGNED NOT NULL AUTO_INCREMENT,
    pIVA CHAR(15) NOT NULL,
    costoOrdine INT UNSIGNED NOT NULL DEFAULT 0,
    contattoSpediz VARCHAR(15) NOT NULL,
    pagato BOOLEAN NOT NULL DEFAULT 0,
    PRIMARY KEY(idVendita),
    FOREIGN KEY(pIVA) REFERENCES Rivendita(pIVA)
);


CREATE TABLE IF NOT EXISTS prod.ContieneVendita(
    idVendita INT UNSIGNED NOT NULL,
    nomeLatino CHAR(100) NOT NULL,
    colore CHAR(30) NOT NULL,
    quantita MEDIUMINT UNSIGNED NOT NULL,
    PRIMARY KEY(idVendita, nomeLatino, colore),
    FOREIGN KEY(idVendita) REFERENCES OrdineVendita(idVendita),
    FOREIGN KEY(nomeLatino, colore) REFERENCES Specie(nomeLatino, colore)
);


CREATE TABLE IF NOT EXISTS prod.OrdineAcquisto(
    idAcquisto INT UNSIGNED NOT NULL AUTO_INCREMENT,
    idFornitore MEDIUMINT UNSIGNED NOT NULL,
    PRIMARY KEY(idAcquisto),
    FOREIGN KEY(idFornitore) REFERENCES Fornitore(idFornitore)
);


CREATE TABLE IF NOT EXISTS prod.ContieneAcquisto(
    idAcquisto INT UNSIGNED NOT NULL,
    nomeLatino CHAR(100) NOT NULL,
    colore CHAR(30) NOT NULL,
    quantita MEDIUMINT UNSIGNED NOT NULL,
    PRIMARY KEY(idAcquisto, nomeLatino, colore),
    FOREIGN KEY(idAcquisto) REFERENCES OrdineAcquisto(idAcquisto),
    FOREIGN KEY(nomeLatino, colore) REFERENCES Specie(nomeLatino, colore)
);







INSERT INTO Specie (nomeLatino, colore, nomeComune, prezzo, esotica, giardAppart) VALUES ("Basilicus", "", "Basilico", 500, 1, "giardino");
INSERT INTO Specie (nomeLatino, colore, nomeComune, prezzo, esotica, giardAppart) VALUES ("Prezzemolus", "", "Prezzemolo", 300, 1, "giardino");




-----------------------------------
--  Correctly adds a new species --
-----------------------------------

DELIMITER $$$

CREATE OR REPLACE PROCEDURE addSpecie(IN nLatino CHAR(100), IN col CHAR(30), IN nComune VARCHAR(50), IN esotic BOOLEAN, giardApp ENUM('giardino', 'appartamento'), IN fiorita BOOLEAN)
    BEGIN
        DECLARE n INT UNSIGNED DEFAULT 3103;

        DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;

        SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
        SET TRANSACTION READ WRITE;
        START TRANSACTION;
            
            IF( fiorita = true ) THEN
                IF( col = "" ) THEN
                    SIGNAL SQLSTATE '45003' SET MESSAGE_TEXT = "Invalid operation";
                END IF;
                SELECT COUNT(*) INTO n
                    FROM Specie
                    WHERE nomeLatino = nLatino
                    AND colore = "";
            ELSE
                IF( col != "" ) THEN
                    SIGNAL SQLSTATE '45003' SET MESSAGE_TEXT = "Invalid operation";
                END IF;
                SELECT COUNT(*) INTO n
                    FROM Specie
                    WHERE nomeLatino = nLatino
                    AND colore != "";
            END IF;

            IF( n != 0 ) THEN
                SIGNAL SQLSTATE '45003' SET MESSAGE_TEXT = "Invalid operation";
            END IF;
                
        
            INSERT INTO Specie (nomeLatino, colore, nomeComune, esotica, giardAppart)
                VALUES(nLatino, col, nComune, esotic, giardApp);

        COMMIT;
    END;
$$$

DELIMITER ;




DELIMITER $$$

CREATE OR REPLACE PROCEDURE setPrezzo(IN nLatino CHAR(100), IN col CHAR(30), IN newPrezzo MEDIUMINT UNSIGNED)
    BEGIN
        DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;

        SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
        SET TRANSACTION READ WRITE;
        START TRANSACTION;

            UPDATE Specie
                SET prezzo = newPrezzo
                WHERE nomeLatino = nLatino
                AND colore = col;

        COMMIT;
    END;
$$$

DELIMITER ;




DELIMITER $$$

CREATE OR REPLACE PROCEDURE editGiacenza(IN nLatino CHAR(100), IN col CHAR(30), IN diffGiacenza INT)
    BEGIN
        DECLARE chck SMALLINT;
        DECLARE oldGiacenza INT;
        DECLARE newGiacenza INT;

        DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;

        SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;
        SET TRANSACTION READ WRITE;
        START TRANSACTION;

            SELECT COUNT(*) INTO chck
                FROM Specie
                WHERE nomeLatino = nLatino
                AND colore = col;

            IF( chck = 0 ) THEN
                SIGNAL SQLSTATE '45001' SET MESSAGE_TEXT = "Species not found";
            END IF;

            SELECT giacenza INTO oldGiacenza
                FROM Specie
                WHERE nomeLatino = nLatino
                AND colore = col;

            SET newGiacenza = oldGiacenza + diffGiacenza;

            IF( newGiacenza < 0 ) THEN
                SIGNAL SQLSTATE '45004' SET MESSAGE_TEXT = "Not enough stock";
            END IF;

			UPDATE Specie
                SET giacenza = newGiacenza
                WHERE nomeLatino = nLatino
                AND colore = col;

        COMMIT;
    END;
$$$

DELIMITER ;






DELIMITER $$$

CREATE OR REPLACE PROCEDURE getFornitoriForSpecie(IN nLatino CHAR(100), IN col CHAR(30))
    BEGIN
        DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;

        SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
        SET TRANSACTION READ ONLY;
        START TRANSACTION;

            SELECT F.idFornitore, F.nomeFornitore
                FROM Fornitore F, Fornisce FS
                WHERE F.idFornitore = FS.idFornitore
                AND FS.nomeLatino = nLatino
                AND FS.colore = col;

        COMMIT;
    END;
$$$

DELIMITER ;





DELIMITER $$$

CREATE OR REPLACE PROCEDURE newBuyOrder(IN idForn MEDIUMINT UNSIGNED, OUT newBuyOrderId INT UNSIGNED)
    BEGIN
        DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;

        SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
        SET TRANSACTION READ WRITE;
        START TRANSACTION;

            INSERT INTO OrdineAcquisto ( idFornitore ) VALUES ( idForn );
            SELECT LAST_INSERT_ID() INTO newBuyOrderId;

        COMMIT;
    END;
$$$

DELIMITER ;




DELIMITER $$$

CREATE OR REPLACE PROCEDURE addSpecieToBuyOrder(IN nLatino CHAR(100), IN col CHAR(30), IN quantita INT UNSIGNED, IN buyOrderId INT UNSIGNED)
    BEGIN
        DECLARE chck SMALLINT UNSIGNED;

        DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;

        SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;
        SET TRANSACTION READ WRITE;
        START TRANSACTION;


            SELECT COUNT(*) INTO chck
                FROM OrdineAcquisto OA, Fornitore F, Fornisce FO
                WHERE OA.idAcquisto = buyOrderId
                AND OA.idFornitore = F.idFornitore
                AND F.idFornitore = FO.idFornitore
                AND FO.nomeLatino = nLatino
                AND FO.colore = col;

            IF( chck = 0 ) THEN
                SIGNAL SQLSTATE '45005' SET MESSAGE_TEXT = "The fornitore of this order does not have the requested specie";
            END IF;

            INSERT INTO ContieneAcquisto ( idAcquisto, nomeLatino, colore, quantita ) VALUES ( buyOrderId, nLatino, col, quantita );

        COMMIT;
    END;
$$$

DELIMITER ;




DELIMITER $$$

CREATE OR REPLACE PROCEDURE newSellOrder(IN iva CHAR(15), IN contatto VARCHAR(15), OUT newSellOrderId INT UNSIGNED)
    BEGIN
        DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;

        SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
        SET TRANSACTION READ WRITE;
        START TRANSACTION;

            INSERT INTO OrdineVendita ( pIVA, contattoSpediz ) VALUES ( iva, contatto );
            SELECT LAST_INSERT_ID() INTO newSellOrderId;

        COMMIT;
    END;
$$$

DELIMITER ;





DELIMITER $$$

CREATE OR REPLACE PROCEDURE addSpecieToSellOrder(IN nLatino CHAR(100), IN col CHAR(30), IN qt INT UNSIGNED, IN sellOrderId INT UNSIGNED)
    BEGIN
        DECLARE price INT UNSIGNED;
        DECLARE giacenzaAtt INT;

        DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;

        SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;
        SET TRANSACTION READ WRITE;
        START TRANSACTION;

            SELECT giacenza, prezzo INTO giacenzaAtt, price
                FROM Specie
                WHERE nomeLatino = nLatino
                AND colore = col;

            IF( giacenzaAtt < qt ) THEN
                SIGNAL SQLSTATE '45006' SET MESSAGE_TEXT = "There isn't enough stock of this species in the wharehouse";
            END IF;

            UPDATE Specie
                SET giacenza = giacenza - qt
                WHERE nomeLatino = nLatino
                AND colore = col;

			UPDATE OrdineVendita
                SET costoOrdine = costoOrdine + ( price * qt )
                WHERE idVendita = sellOrderId;

            INSERT INTO ContieneVendita ( idVendita, nomeLatino, colore, quantita )
                VALUES ( sellOrderId, nLatino, col, qt );

        COMMIT;
    END;
$$$

DELIMITER ;




DELIMITER $$$

CREATE OR REPLACE PROCEDURE getCostoOrdine(IN sellId INT UNSIGNED, OUT cost INT UNSIGNED)
    BEGIN
        DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;

        SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
        SET TRANSACTION READ ONLY;
        START TRANSACTION;

            SELECT costoOrdine INTO cost
            FROM OrdineVendita
            WHERE idVendita = sellId;

        COMMIT;
    END;
$$$

DELIMITER ;



DELIMITER $$$

CREATE OR REPLACE PROCEDURE confirmPayment(IN sellId INT UNSIGNED)
    BEGIN
        DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;

        SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
        SET TRANSACTION READ WRITE;
        START TRANSACTION;

            UPDATE OrdineVendita
                SET pagato = 1
                WHERE idVendita = sellId;

        COMMIT;
    END;
$$$

DELIMITER ;














-- DA FARE PER BENE

CREATE OR REPLACE ROLE amministratore;
GRANT EXECUTE ON PROCEDURE prod.editGiacenza TO amministratore;
GRANT SELECT,INSERT,UPDATE,DELETE ON mysql.* TO amministratore WITH GRANT OPTION;
GRANT SELECT,INSERT,UPDATE ON prod.* TO amministratore WITH GRANT OPTION;

CREATE OR REPLACE ROLE manager WITH ADMIN amministratore;
GRANT SELECT,INSERT ON prod.Specie TO manager;
GRANT UPDATE(prezzo) ON prod.Specie TO manager;

CREATE OR REPLACE ROLE magazzino WITH ADMIN amministratore;
GRANT SELECT ON prod.Specie TO magazzino;
GRANT UPDATE(giacenza) ON prod.Specie TO magazzino;
GRANT SELECT ON prod.Fornisce TO magazzino;
GRANT SELECT ON prod.Fornitore TO magazzino;
GRANT SELECT,INSERT ON prod.OrdineAcquisto TO magazzino;
GRANT SELECT,INSERT ON prod.ContieneAcquisto TO magazzino;

CREATE OR REPLACE ROLE segreteria WITH ADMIN amministratore;
GRANT SELECT ON prod.Specie TO segreteria;
GRANT SELECT ON prod.Rivendita TO segreteria;
GRANT SELECT,INSERT ON prod.OrdineVendita TO segreteria;
GRANT SELECT,INSERT ON prod.ContieneVendita TO segreteria;





CREATE OR REPLACE USER admin IDENTIFIED BY 'CHANGEME';
GRANT amministratore TO admin;
SET DEFAULT ROLE amministratore FOR admin;




CREATE OR REPLACE USER test IDENTIFIED BY 'test';
GRANT segreteria TO test;
SET DEFAULT ROLE segreteria FOR test;


