
USE cinema;

-- Création de la table personne

CREATE TABLE personne(
 numpersonne mediumint unsigned primary key,
 prenom varchar(15) not null,
 nom varchar(15) not null,
 datenais date,
 nationalite varchar(20),
 adresse varchar(50),
 cp varchar(5),
 ville varchar(15),
 telephone varchar(10))
ENGINE=InnoDB;

DESC personne;

-- Création de la table Genre

CREATE TABLE genre(
 numgenre tinyint unsigned primary key,
 libellegenre varchar(50))
ENGINE=InnoDB;

DESC genre;
-- Création de la table Cinema



CREATE TABLE cinema(
 numcinema smallint unsigned primary key,
 nom varchar(60),
 adresse varchar(50),
 cp  varchar(5),
 ville varchar(50),
 telephone varchar(10),
 Compagnie varchar(15))
ENGINE=InnoDB;

DESC cinema;

-- Création de la table Acteur


CREATE TABLE acteur(
 NumActeur MediumInt Unsigned primary key,
 NumPersonne MediumInt Unsigned,
 Agent MediumInt Unsigned,
 Specialite tinyint unsigned,
 Taille TinyInt Unsigned,
 Poids TinyInt Unsigned,
 CONSTRAINT FK_acteur_personne FOREIGN KEY (numpersonne) REFERENCES personne(numpersonne),
 CONSTRAINT FK_acteur_agent FOREIGN KEY (agent) REFERENCES personne(numpersonne),
 CONSTRAINT FK_acteur_genre FOREIGN KEY (specialite) REFERENCES genre(numgenre))
ENGINE=InnoDB;


DESC acteur;

-- Création de la table Salle


CREATE TABLE salle(
 NumCinema SmallInt Unsigned,
 NumSalle TinyInt Unsigned,
 TailleEcran TinyInt Unsigned,
 NbPlaces SmallInt Unsigned,
 PRIMARY KEY (NumCinema,NumSalle),
 CONSTRAINT FK_salle_cinema FOREIGN KEY (NumCinema) REFERENCES cinema(numcinema))
ENGINE=InnoDB;


DESC salle;

-- Création de la table 


CREATE TABLE film(
 Numfilm Int Unsigned PRIMARY KEY,
 Titre  Varchar(100),
 Genre  TinyInt Unsigned,
 Annee  Year,
 Longueur Time,
 Budget  Int  Unsigned,
 Nb_spec_cumule Int,
 Realisateur MediumInt Unsigned,
 Salaire_real Int,
 CONSTRAINT FK_film_personne FOREIGN KEY (Realisateur) REFERENCES personne(numpersonne),
 CONSTRAINT FK_film_genre FOREIGN KEY (genre) REFERENCES genre(numgenre))
ENGINE=InnoDB;


DESC film;

-- Création de la table Distribution


CREATE TABLE distribution(
 NumFilm Int Unsigned ,
 NumActeur MediumInt Unsigned,
 role  Varchar(15),
 Salaire Int,
 PRIMARY KEY (numfilm,numacteur,role),
 CONSTRAINT FK_distribution_film FOREIGN KEY (numfilm) REFERENCES film(numfilm),
 CONSTRAINT FK_distribution_acteur FOREIGN KEY (numacteur) REFERENCES acteur(numacteur))
ENGINE=InnoDB;


DESC distribution;


-- Création de la table Proggrammation



CREATE TABLE programmation(
 NumFilm Int Unsigned,
 NumCinema SmallInt Unsigned,
 NumSalle TinyInt Unsigned,
 Date_Deb Date,
 Date_fin Date,
 Horaire Time,
 Prix  Numeric(5,2),
 PRIMARY KEY (NumFilm,NumCinema,NumSalle,Date_Deb),
 CONSTRAINT FK_programmation_salle FOREIGN KEY (NumCinema,NumSalle) REFERENCES salle(NumCinema,NumSalle),
 CONSTRAINT FK_programmation_film FOREIGN KEY (numfilm) REFERENCES film(numfilm))
ENGINE=InnoDB;
 

DESC programmation;

SHOW tables;

SELECT count(*) from personne;
SELECT count(*) from genre;
SELECT count(*) from cinema;
SELECT count(*) from acteur;
SELECT count(*) from salle;
SELECT count(*) from film;
SELECT count(*) from distribution;
SELECT count(*) from programmation;


