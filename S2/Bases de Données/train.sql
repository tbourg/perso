create table ligne(
nuligne number(2),
rang number(2),
nomgare varchar2(20),
constraint pk_ligne primary key(nuligne,rang)
);
create table wagon(
nuwagon number(4) primary key,
typewagon varchar2(20),
poidsvide number(2),
capacite number(2),
etat varchar2(20),
nomgare varchar2(20)
);
create table train(
nutrain number(4),
nuwagon number(4),
constraint pk_train primary key(nutrain,nuwagon),
constraint fk_wagon foreign key(nuwagon) references wagon(nuwagon)
);
create table trafic(
nutrain number(4),
nuligne number(2),
nujour date,
constraint pk_trafic primary key(nutrain,nuligne,nujour)
);
create table reseau(
nomgareorigine varchar2(20),
nomgaredestination varchar2(20),
nuligne number(2),
nomgarearrivee varchar2(20),
constraint pk_reseau primary key(nomgareorigine ,nomgaredestination ,nuligne ,nomgarearrivee) 
);
insert into ligne values (10,1,'nantes') ;
insert into ligne values (10,2,'angers') ;
insert into ligne values (10,3,'saumur') ;
insert into ligne values (10,4,'tours') ;
insert into ligne values (11,1,'nantes') ;
insert into ligne values (11,2,'tours') ;
insert into ligne values (13,1,'angers') ;
insert into ligne values (13,2,'paris') ;
insert into ligne values (13,3,'lyon') ;
insert into ligne values (13,4,'beziers') ;
insert into ligne values (15,1,'tours') ;
insert into ligne values (15,2,'bordeaux') ;
insert into ligne values (16,1,'orleans') ;
insert into ligne values (16,2,'tours') ;
insert into ligne values (16,3,'poitiers') ;
insert into ligne values (21,1,'bordeaux') ;
insert into ligne values (21,2,'toulouse') ;
insert into ligne values (30,1,'toulouse') ;
insert into ligne values (30,2,'beziers') ;
commit;
insert into wagon values (1004 , 'frigo' , 10 , 30 , 'libre', 'tours');
insert into wagon values (1104 , 'citerne' , 6 , 15 , 'charge', 'paris');
insert into wagon values (1105 , 'frigo' , 10 , 30 , 'libre', 'orleans');
insert into wagon values (1106 , 'frigo' , 10 , 30 , 'charge', 'tours');
insert into wagon values (2019 , 'plat' , 7 , 20 , 'libre', 'angers');
commit;
insert into train values (4002 , 1104) ;
insert into train values (4002 , 1105) ;
insert into train values (4002 , 1106) ;
insert into train values (4051 , 1004) ;
insert into train values (4051 , 2019) ;
commit;
insert into trafic values (4002 , 10 , TO_DATE('12/03/13' , 'DD/MM/YY')) ;
insert into trafic values (4002 , 10 , TO_DATE('14/03/13' , 'DD/MM/YY')) ;
insert into trafic values (4051 , 13 , TO_DATE('12/03/13' , 'DD/MM/YY')) ;
commit ;
insert into reseau values ('angers','beziers',10,'tours');
insert into reseau values ('bordeaux','beziers',21,'toulouse');
insert into reseau values ('saumur','tours',10,'tours');
insert into reseau values ('toulouse','beziers',30,'beziers');
insert into reseau values ('tours','beziers',15,'bordeaux');
insert into reseau values ('angers','beziers',13,'paris');
insert into reseau values ('paris','beziers',13,'lyon');
insert into reseau values ('lyon','beziers',13,'beziers');
commit;

--1--
select nuwagon from wagon where nomgare = 'tours' and typewagon = 'frigo' and capacite > 10;

--2--
select wagon.nuwagon, typewagon from wagon left join train on wagon.NUWAGON = train.NUWAGON where nutrain = 4002;

--3--
select nuligne from ligne where nomgare = 'tours' and rang != 1;

--4--
select nutrain from trafic left join reseau on trafic.NULIGNE = reseau.NULIGNE where nujour = '12/03/2013' and nomgareorigine = 'angers' and nomgaredestination = 'beziers';

--5--
select nuwagon from train left join trafic on train.nutrain = trafic.nutrain left join reseau on trafic.NULIGNE = reseau.NULIGNE where nujour = '12/03/2013' and nomgareorigine = 'angers' and nomgaredestination = 'beziers';

--6--
select nuligne from trafic group by nuligne having count(distinct nujour)=(select count(distinct nujour) from trafic);

--7--
select distinct nomgarearrivee from reseau where nuligne =10;

--8--
select nutrain, sum(poidsvide+capacite) as poids_en_charge from train left join wagon on wagon.NUWAGON = train.NUWAGON where nutrain = 4002 group by nutrain;

--9--
select distinct nomgare from reseau left join ligne on reseau.NULIGNE = ligne.NULIGNE where nomgareorigine = 'angers' and nomgaredestination = 'beziers';

--10--
select nutrain, count(*) from train group by nutrain;

--11--
select nutrain from train group by nutrain having count(*) >= 2;

--12--
create view train2 as select nutrain, wagon.nuwagon, typewagon, capacite from train left join wagon on wagon.NUWAGON = train.NUWAGON;