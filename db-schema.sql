
CREATE DATABASE IF NOT EXISTS prod;
USE prod;


CREATE TABLE IF NOT EXISTS prod.Specie(
    nomeLatino CHAR(100) NOT NULL,
    colore CHAR(30) NOT NULL,
    nomeComune VARCHAR(50) NOT NULL,
    giacenza INT NOT NULL DEFAULT 0,
    prezzo INT UNSIGNED NOT NULL DEFAULT 0,
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
    idFornitore INT UNSIGNED NOT NULL AUTO_INCREMENT,
    nomeFornitore VARCHAR(50) NOT NULL,
    codFisc VARCHAR(20) NOT NULL,
    PRIMARY KEY(idFornitore)
);


CREATE TABLE IF NOT EXISTS prod.IndirizzoFornitore(
    idFornitore INT UNSIGNED NOT NULL,
    indirizzo VARCHAR(50) NOT NULL,
    PRIMARY KEY(idFornitore, indirizzo),
    FOREIGN KEY(idFornitore) REFERENCES Fornitore(idFornitore)
);


CREATE TABLE IF NOT EXISTS prod.Fornisce(
    idFornitore INT UNSIGNED NOT NULL,
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
    quantita INT UNSIGNED NOT NULL,
    PRIMARY KEY(idVendita, nomeLatino, colore),
    FOREIGN KEY(idVendita) REFERENCES OrdineVendita(idVendita),
    FOREIGN KEY(nomeLatino, colore) REFERENCES Specie(nomeLatino, colore)
);


CREATE TABLE IF NOT EXISTS prod.OrdineAcquisto(
    idAcquisto INT UNSIGNED NOT NULL AUTO_INCREMENT,
    idFornitore INT UNSIGNED NOT NULL,
    PRIMARY KEY(idAcquisto),
    FOREIGN KEY(idFornitore) REFERENCES Fornitore(idFornitore)
);


CREATE TABLE IF NOT EXISTS prod.ContieneAcquisto(
    idAcquisto INT UNSIGNED NOT NULL,
    nomeLatino CHAR(100) NOT NULL,
    colore CHAR(30) NOT NULL,
    quantita INT UNSIGNED NOT NULL,
    PRIMARY KEY(idAcquisto, nomeLatino, colore),
    FOREIGN KEY(idAcquisto) REFERENCES OrdineAcquisto(idAcquisto),
    FOREIGN KEY(nomeLatino, colore) REFERENCES Specie(nomeLatino, colore)
);





-----------------------------
-- Returns all the species --
-----------------------------

DELIMITER $$$

CREATE PROCEDURE IF NOT EXISTS getSpecie()
    BEGIN
        DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;

        SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
        SET TRANSACTION READ ONLY;
        START TRANSACTION;

            SELECT nomeLatino,
                colore,
                nomeComune,
                giacenza,
                prezzo,
                esotica,
                giardAppart,
                CASE WHEN colore = '' THEN 'verde' ELSE 'fiorita' END AS verdeFiorita
                FROM Specie;

        COMMIT;
    END;
$$$

DELIMITER ;



-----------------------------------
--  Correctly adds a new species --
-----------------------------------

DELIMITER $$$

CREATE PROCEDURE IF NOT EXISTS addSpecie(IN nLatino CHAR(100), IN col CHAR(30), IN nComune VARCHAR(50), IN esotic BOOLEAN, IN giardino BOOLEAN, IN fiorita BOOLEAN)
    BEGIN
        DECLARE n INT UNSIGNED DEFAULT 3103;
        DECLARE giardApp CHAR(15);

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
                    SIGNAL SQLSTATE '45003' SET MESSAGE_TEXT = "Cannot have an empty string to a specie fiorita";
                END IF;
                SELECT COUNT(*) INTO n
                    FROM Specie
                    WHERE nomeLatino = nLatino
                    AND colore = "";
            ELSE
                IF( col != "" ) THEN
                    SIGNAL SQLSTATE '45103' SET MESSAGE_TEXT = "Color of a specie verde has to be an empty string";
                END IF;
                SELECT COUNT(*) INTO n
                    FROM Specie
                    WHERE nomeLatino = nLatino
                    AND colore != "";
            END IF;

            IF( n != 0 ) THEN
                SIGNAL SQLSTATE '45203' SET MESSAGE_TEXT = "Wrong specie's type";
            END IF;


            if( giardino = true ) THEN
                SET giardApp = 'giardino';
            ELSE
                SET giardApp = 'appartamento';
            END IF;
                
        
            INSERT INTO Specie (nomeLatino, colore, nomeComune, esotica, giardAppart)
                VALUES(nLatino, col, nComune, esotic, giardApp);

        COMMIT;
    END;
$$$

DELIMITER ;



-----------------------------------
--  Sets a new price to a specie --
-----------------------------------

DELIMITER $$$

CREATE PROCEDURE IF NOT EXISTS setPrezzo(IN nLatino CHAR(100), IN col CHAR(30), IN newPrezzo INT UNSIGNED)
    BEGIN
        DECLARE chck SMALLINT;

        DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;

        SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
        SET TRANSACTION READ WRITE;
        START TRANSACTION;

            SELECT COUNT(*) INTO chck
                FROM Specie
                WHERE nomeLatino = nLatino
                AND colore = col;

            IF( chck = 0 ) THEN
                SIGNAL SQLSTATE '45001' SET MESSAGE_TEXT = "Species not found";
            END IF;

            UPDATE Specie
                SET prezzo = newPrezzo
                WHERE nomeLatino = nLatino
                AND colore = col;

        COMMIT;
    END;
$$$

DELIMITER ;



---------------------------------------------------------
-- Aggiorna in modo relativo le giacenza di una specie --
---------------------------------------------------------

DELIMITER $$$

CREATE PROCEDURE IF NOT EXISTS editGiacenza(IN nLatino CHAR(100), IN col CHAR(30), IN diffGiacenza INT)
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



---------------------------------------------------------
-- Returns all the fornitore that sells a given specie --
---------------------------------------------------------

DELIMITER $$$

CREATE PROCEDURE IF NOT EXISTS getFornitoriForSpecie(IN nLatino CHAR(100), IN col CHAR(30))
    BEGIN
        DECLARE chck SMALLINT;

        DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;

        SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
        SET TRANSACTION READ ONLY;
        START TRANSACTION;

            SELECT COUNT(*) INTO chck
                FROM Specie
                WHERE nomeLatino = nLatino
                AND colore = col;

            IF( chck = 0 ) THEN
                SIGNAL SQLSTATE '45001' SET MESSAGE_TEXT = "Species not found";
            END IF;

            SELECT F.idFornitore, F.nomeFornitore
                FROM Fornitore F, Fornisce FS
                WHERE F.idFornitore = FS.idFornitore
                AND FS.nomeLatino = nLatino
                AND FS.colore = col;

        COMMIT;
    END;
$$$

DELIMITER ;



-----------------------------
-- Creates a new buy order --
-----------------------------

DELIMITER $$$

CREATE PROCEDURE IF NOT EXISTS newBuyOrder(IN idForn INT UNSIGNED, OUT newBuyOrderId INT UNSIGNED)
    BEGIN
        DECLARE chck SMALLINT;

        DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;

        SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
        SET TRANSACTION READ WRITE;
        START TRANSACTION;

            SELECT COUNT(*) INTO chck
                FROM Fornitore
                WHERE idFornitore = idForn;

            IF( chck = 0 ) THEN
                SIGNAL SQLSTATE '45007' SET MESSAGE_TEXT = "Fornitore not found";
            END IF;

            INSERT INTO OrdineAcquisto ( idFornitore ) VALUES ( idForn );
            SELECT LAST_INSERT_ID() INTO newBuyOrderId;

        COMMIT;
    END;
$$$

DELIMITER ;



----------------------------------
-- Adds a specie to a buy order --
----------------------------------

DELIMITER $$$

CREATE PROCEDURE IF NOT EXISTS addSpecieToBuyOrder(IN nLatino CHAR(100), IN col CHAR(30), IN qt INT UNSIGNED, IN buyOrderId INT UNSIGNED)
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
                FROM Specie
                WHERE nomeLatino = nLatino
                AND colore = col;

            IF( chck = 0 ) THEN
                SIGNAL SQLSTATE '45001' SET MESSAGE_TEXT = "Species not found";
            END IF;

            SELECT COUNT(*) INTO chck
                FROM OrdineAcquisto OA, Fornitore F, Fornisce FO
                WHERE OA.idAcquisto = buyOrderId
                AND OA.idFornitore = F.idFornitore
                AND F.idFornitore = FO.idFornitore
                AND FO.nomeLatino = nLatino
                AND FO.colore = col;

            IF( chck = 0 ) THEN
                SIGNAL SQLSTATE '45005' SET MESSAGE_TEXT = "The fornitore of this order does not have the requested specie, or the order id is wrong";
            END IF;

            SELECT COUNT(*) INTO chck
                FROM ContieneAcquisto
                WHERE idAcquisto = buyOrderId
                AND nomeLatino = nLatino
                AND colore = col;

            IF( chck = 0 ) THEN
                INSERT INTO ContieneAcquisto ( idAcquisto, nomeLatino, colore, quantita )
                    VALUES ( buyOrderId, nLatino, col, qt );
            ELSE
                UPDATE ContieneAcquisto
                    SET quantita = quantita + qt
                    WHERE idAcquisto = buyOrderId
                    AND nomeLatino = nLatino
                    AND colore = col;
            END IF;

        COMMIT;
    END;
$$$

DELIMITER ;



------------------------------
-- Creates a new sell order --
------------------------------

DELIMITER $$$

CREATE PROCEDURE IF NOT EXISTS newSellOrder(IN iva CHAR(15), IN contatto VARCHAR(15), OUT newSellOrderId INT UNSIGNED)
    BEGIN
        DECLARE chck SMALLINT UNSIGNED;

        DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;

        SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
        SET TRANSACTION READ WRITE;
        START TRANSACTION;

            SELECT COUNT(*) INTO chck
                FROM Rivendita
                WHERE pIVA = iva;

            IF( chck = 0 ) THEN
                SIGNAL SQLSTATE '45008' SET MESSAGE_TEXT = "Rivendita not found";
            END IF;

            INSERT INTO OrdineVendita ( pIVA, contattoSpediz ) VALUES ( iva, contatto );
            SELECT LAST_INSERT_ID() INTO newSellOrderId;

        COMMIT;
    END;
$$$

DELIMITER ;



----------------------------------
-- Adds a specie to a buy order --
----------------------------------

DELIMITER $$$

CREATE PROCEDURE IF NOT EXISTS addSpecieToSellOrder(IN nLatino CHAR(100), IN col CHAR(30), IN qt INT UNSIGNED, IN sellOrderId INT UNSIGNED)
    BEGIN
        DECLARE chck SMALLINT UNSIGNED;
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

            SELECT COUNT(*) INTO chck
                FROM Specie
                WHERE nomeLatino = nLatino
                AND colore = col;

            IF( chck = 0 ) THEN
                SIGNAL SQLSTATE '45001' SET MESSAGE_TEXT = "Species not found";
            END IF;

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

            SELECT COUNT(*) INTO chck
                FROM ContieneVendita
                WHERE idVendita = sellOrderId
                AND nomeLatino = nLatino
                AND colore = col;

            IF( chck = 0 ) THEN
                INSERT INTO ContieneVendita ( idVendita, nomeLatino, colore, quantita )
                    VALUES ( sellOrderId, nLatino, col, qt );
            ELSE
                UPDATE ContieneVendita
                    SET quantita = quantita + qt
                    WHERE idVendita = sellOrderId
                    AND nomeLatino = nLatino
                    AND colore = col;
            END IF;

        COMMIT;
    END;
$$$

DELIMITER ;



--------------------------------------------------
-- Returns the total cost of a given sell order --
--------------------------------------------------

DELIMITER $$$

CREATE PROCEDURE IF NOT EXISTS getCostoOrdine(IN sellId INT UNSIGNED, OUT cost INT UNSIGNED)
    BEGIN
        DECLARE chck SMALLINT UNSIGNED;

        DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;

        SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
        SET TRANSACTION READ ONLY;
        START TRANSACTION;

            SELECT COUNT(*) INTO chck
                FROM OrdineVendita
                WHERE idVendita = sellId;

            IF( chck = 0 ) THEN
                SIGNAL SQLSTATE '45009' SET MESSAGE_TEXT = "There isn't a sell order with this id";
            END IF;

            SELECT costoOrdine INTO cost
                FROM OrdineVendita
                WHERE idVendita = sellId;

        COMMIT;
    END;
$$$

DELIMITER ;



--------------------------
-- Confirms a buy order --
--------------------------

DELIMITER $$$

CREATE PROCEDURE IF NOT EXISTS confirmPayment(IN sellId INT UNSIGNED)
    BEGIN
        DECLARE chck SMALLINT UNSIGNED;

        DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;

        SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
        SET TRANSACTION READ WRITE;
        START TRANSACTION;


            SELECT COUNT(*) INTO chck
                FROM OrdineVendita
                WHERE idVendita = sellId;

            IF( chck = 0 ) THEN
                SIGNAL SQLSTATE '45009' SET MESSAGE_TEXT = "There isn't a sell order with this id";
            END IF;

            UPDATE OrdineVendita
                SET pagato = 1
                WHERE idVendita = sellId;

        COMMIT;
    END;
$$$

DELIMITER ;



------------------------------------------
-- Changes password of the current user --
------------------------------------------

DELIMITER $$$

CREATE PROCEDURE IF NOT EXISTS changePassword(IN newPsw CHAR(100))
    BEGIN

        DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;

        SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
        SET TRANSACTION READ WRITE;
        START TRANSACTION;

            SET @sql = CONCAT('SET PASSWORD FOR ''',
                SUBSTRING_INDEX(USER(), '@', 1),
                ''' = PASSWORD(''',
                REPLACE(newPsw,'''',''''''),
                ''')'
            );

            EXECUTE IMMEDIATE @sql;
            SET @sql = '';

        COMMIT;
    END;
$$$

DELIMITER ;



-------------------
-- Adds new user --
-------------------

DELIMITER $$$

CREATE PROCEDURE IF NOT EXISTS newUser(IN username CHAR(128), IN password CHAR(100), IN defRole ENUM('manager','magazzino','segreteria'))
    BEGIN

        DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;

        SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;
        SET TRANSACTION READ WRITE;
        START TRANSACTION;

            SET @sql1 = CONCAT('CREATE OR REPLACE USER ''',
                REPLACE(username,'''',''''''),
                ''' IDENTIFIED BY ''',
                REPLACE(password,'''',''''''),
                ''''
            );

            SET @sql2 = CONCAT('GRANT ''',
                REPLACE(defRole,'''',''''''),
                ''' TO ''',
                REPLACE(username,'''',''''''),
                ''''
            );

            SET @sql3 = CONCAT('SET DEFAULT ROLE ''',
                REPLACE(defRole,'''',''''''),
                ''' FOR ''',
                REPLACE(username,'''',''''''),
                ''''
            );

            EXECUTE IMMEDIATE @sql1;
            EXECUTE IMMEDIATE @sql2;
            EXECUTE IMMEDIATE @sql3;

            SET @sql1 = '';
            SET @sql2 = '';
            SET @sql3 = '';

        COMMIT;
    END;
$$$

DELIMITER ;



---------------------
-- Deletes an user --
---------------------

DELIMITER $$$

CREATE PROCEDURE IF NOT EXISTS dropUser(IN username CHAR(128))
    BEGIN

        DECLARE EXIT HANDLER FOR SQLEXCEPTION
        BEGIN
            ROLLBACK;
            RESIGNAL;
        END;

        SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
        SET TRANSACTION READ WRITE;
        START TRANSACTION;

           SET @sql = CONCAT('DROP USER ''',
                REPLACE(username,'''',''''''),
                ''''
            );

            EXECUTE IMMEDIATE @sql;
            SET @sql = '';

        COMMIT;
    END;
$$$

DELIMITER ;





CREATE OR REPLACE ROLE amministratore;
GRANT EXECUTE ON PROCEDURE prod.getSpecie TO amministratore WITH GRANT OPTION;
GRANT EXECUTE ON PROCEDURE prod.addSpecie TO amministratore WITH GRANT OPTION;
GRANT EXECUTE ON PROCEDURE prod.setPrezzo TO amministratore WITH GRANT OPTION;
GRANT EXECUTE ON PROCEDURE prod.editGiacenza TO amministratore WITH GRANT OPTION;
GRANT EXECUTE ON PROCEDURE prod.getFornitoriForSpecie TO amministratore WITH GRANT OPTION;
GRANT EXECUTE ON PROCEDURE prod.newBuyOrder TO amministratore WITH GRANT OPTION;
GRANT EXECUTE ON PROCEDURE prod.addSpecieToBuyOrder TO amministratore WITH GRANT OPTION;
GRANT EXECUTE ON PROCEDURE prod.newSellOrder TO amministratore WITH GRANT OPTION;
GRANT EXECUTE ON PROCEDURE prod.addSpecieToSellOrder TO amministratore WITH GRANT OPTION;
GRANT EXECUTE ON PROCEDURE prod.getCostoOrdine TO amministratore WITH GRANT OPTION;
GRANT EXECUTE ON PROCEDURE prod.confirmPayment TO amministratore WITH GRANT OPTION;
GRANT EXECUTE ON PROCEDURE prod.changePassword TO amministratore WITH GRANT OPTION;
GRANT EXECUTE ON PROCEDURE prod.newUser TO amministratore;
GRANT EXECUTE ON PROCEDURE prod.dropUser TO amministratore;

CREATE OR REPLACE ROLE manager;
GRANT EXECUTE ON PROCEDURE prod.getSpecie TO manager;
GRANT EXECUTE ON PROCEDURE prod.addSpecie TO manager;
GRANT EXECUTE ON PROCEDURE prod.setPrezzo TO manager;
GRANT EXECUTE ON PROCEDURE prod.changePassword TO manager;


CREATE OR REPLACE ROLE magazzino;
GRANT EXECUTE ON PROCEDURE prod.getSpecie TO magazzino;
GRANT EXECUTE ON PROCEDURE prod.editGiacenza TO magazzino;
GRANT EXECUTE ON PROCEDURE prod.getFornitoriForSpecie TO magazzino;
GRANT EXECUTE ON PROCEDURE prod.newBuyOrder TO magazzino;
GRANT EXECUTE ON PROCEDURE prod.addSpecieToBuyOrder TO magazzino;
GRANT EXECUTE ON PROCEDURE prod.changePassword TO manager;


CREATE OR REPLACE ROLE segreteria;
GRANT EXECUTE ON PROCEDURE prod.getSpecie TO segreteria;
GRANT EXECUTE ON PROCEDURE prod.newSellOrder TO segreteria;
GRANT EXECUTE ON PROCEDURE prod.addSpecieToSellOrder TO segreteria;
GRANT EXECUTE ON PROCEDURE prod.getCostoOrdine TO segreteria;
GRANT EXECUTE ON PROCEDURE prod.confirmPayment TO segreteria;
GRANT EXECUTE ON PROCEDURE prod.changePassword TO manager;





CREATE OR REPLACE USER admin IDENTIFIED BY 'CHANGEME';
GRANT amministratore TO admin;
SET DEFAULT ROLE amministratore FOR admin;
