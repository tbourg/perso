create table vins ( nvin number constraint pk_nvin_vins primary key not null, 
                        nomcru varchar2(10) not null,
                        annee number default 1980 not null,
                        degres number(3,1),
                        prix number);

create table recolte (nvrec number Constraint fK_nvec_recolte references vins(nvin) not null,
                      refprod number not null ,
                      datrec date,
                      qtrec number default 0 not null,
                      constraint pk_recolte primary key(nvrec,refprod,datrec));
                
create table producteurs (refprod number constraint pk_producteurs primary key,
                          nomprod varchar2(12) default ' ',
                          region varchar2(10) default ' ');
                          
create table acheteurs (numach number constraint pk_acheteurs primary key,
                          nomach varchar2(10) not null,
                          typbuv varchar2(5) default ' ' not null constraint ck_typbuv_acheteur check (typbuv in ('PETIT','MOYEN','GRAND')));
                          
create table achat (nvin number not null,
                    nachet number not null constraint fk_achat references acheteurs(numach),
                    qte number default 0 not null,
                    cpt number constraint pk_achat primary key);
                
alter table recolte modify (refprod number constraint fk_recolte references producteurs(refprod));


alter table vins modify(annee number constraint ck_annee_vins check (annee>1980));
alter table achat modify(qte number constraint ck_qte_achat check (qte>0));

alter table acheteurs drop constraint ck_typbuv_acheteur;

alter table vins modify(prix number(4,2));

commit;

insert into acheteurs values (1,'Hardy', 'GROS');
insert into acheteurs values (2,'Duchenne','MOYEN');
insert into acheteurs values (3,'Bernardin','PETIT');
insert into acheteurs values (4,'Dreyfus','PETIT');

insert into producteurs values (1,'Knock','BEAUJOLAIS');
insert into producteurs values (2,'Marchand','LOIRE');
insert into producteurs values (3,'Bouteiller','BORDEAUX');
insert into producteurs values (4,'Smith','SUD-OUEST');

insert into achat values (1,9,12,1);
insert into achat values (1,4,7,2);
insert into achat values (1,1,10,3);
insert into achat values (1,5,12,4);
insert into achat values (1,6,9,5);

insert into vins values (1,'St Amour', 1983, 11.5, 31);
insert into vins values (2,'Pomerol',1982,11.6,29);
insert into vins values (3,'Chenas',1986,11.3,32.5);
insert into vins values (4,'Gamay',1985,11.4,26);
insert into vins values (5,'Riesling',1983,11.9, 23.5);

insert into recolte values(1,1,'15/09/1983',62);
insert into recolte values (1,6,'17/09/1983',158);
insert into recolte values (2,3,'29/08/1982',25);
insert into recolte values (3,11,'05/10/1986',130);

commit;





select nomcru from vins order by nomcru;

select distinct region from producteurs;

select refprod, nomprod from producteurs 
where region in ('SAVOIE', 'BEAUJOLAIS');

select nomach as sobre from acheteurs 
where typbuv = 'PETIT';

select nvin as vin, nachet from achat 
order by nachet desc, vin;

create table jeune as select nomcru, degres from vins 
where annee>1985;
select * from jeune;

select nachet from achat 
where nvin = any(select nvin from vins where prix>30);

select nvrec, nomprod, region from recolte, producteurs 
where recolte.refprod = producteurs.refprod and qtrec>100;

select distinct nvrec from recolte 
where refprod = any (select refprod from producteurs where region = 'BOURGOGNE');

select count(*) as Nbre_Buveurs_Moy from acheteurs 
where typbuv = 'MOYEN';

select count(*) as nachet, sum(qte) as qte_acheté from achat 
where nvin = 7;

select sum(qte*prix) from vins, acheteurs, achat 
where vins.nvin=achat.nvin and achat.nachet=acheteurs.numach 
group by numach 
having numach = (select numach from acheteurs where nomach = 'Hardy'); 

select avg(prix) from vins, achat 
where vins.nvin=achat.nvin 
group by nachet 
having nachet = 9;

select nomprod from producteurs 
where refprod = any(select refprod from recolte where qtrec=45);

select nomach from acheteurs 
where numach = any(select a1.nachet from achat a1, achat a2, achat a3 where a1.nachet=a2.nachet and a2.nachet = a3.nachet and a1.nvin = 1 and a2.nvin = 7 and a3.nvin = 12);

select nvin from vins 
minus 
select distinct nvin from achat;

select distinct nomcru from vins, recolte,achat,acheteurs 
where nvrec = vins.nvin and vins.nvin = achat.nvin and nachet = numach and datrec >'01/01/1984' and typbuv !='PETIT';

select nomach as gros_acheteurs from acheteurs 
where numach = 
any(select nachet from achat 
group by nachet 
having sum(qte) > 
(select avg(qte_tot) from (select nachet, sum(qte) as qte_tot from achat group by nachet)));

select numach from acheteurs 
minus 
select distinct nachet as numach from achat 
where nvin = any(select nvin from vins where nomcru = 'St Amour');
