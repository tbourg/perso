--1--
select * from film order by titre;

--2--
select distinct ville from personne order by ville;

--3--
select titre, budget, 'Film à petit budget' as type from film where budget <= 1000000;

--4--
select * from personne where adr like '%Avenue%';

--5--
select * from personne where tel is null;

--6--
select distinct nom, pren  from film
left join personne on numpers = real
where ville like 'N%';

--7--
select titre, annee, nom from film
left join personne on numpers = real
where nom != 'Spielberg';

--8--
select nom, pren, titre, round(sal_real/longueur) from film
left join personne on numpers = real;

--9--
select titre, nom, pren, sal from film
left join distribution on distribution.numfilm = film.numfilm
left join acteur on distribution.numact = acteur.numact
left join personne on acteur.numpers = personne.numpers
where sal is not null order by titre, sal desc;

--10--
select nom, pren, sal/1.03, titre from acteur
left join distribution on acteur.numact = distribution.numact
left join personne on acteur.numpers = personne.numpers
left join film on film.numfilm = distribution.numfilm
where titre is not null;

--11--
select act.nom, act.pren from personne act
left join acteur on act.numpers = acteur.numpers
left join distribution on acteur.numact = distribution.numact
left join film on distribution.numfilm = film.numfilm
left join personne real on film.real= real.numpers
where real.nom = 'Spielberg'
and acteur."specialite" = (
select numgenre from genre
where libellegenre = 'Drame');

--12--
select distinct act.nom, act.pren from personne act
left join acteur on act.numpers = acteur.numpers
left join distribution on acteur.numact = distribution.numact
left join film on distribution.numfilm = film.numfilm
left join personne real on film.real= real.numpers
where act.numpers = real.numpers;

--13--
select cinema.nom, titre from cinema
left join salle on salle.numcine = cinema.numcine
left join programmation on salle.numcine = programmation.numcine and salle.numsalle = programmation.numsalle
left join film on programmation.numfilm = film.numfilm
left join personne on film.real = personne.numpers
where comp = 'indep'
and cinema.ville = 'Bordeaux'
and taille > 30
and nbplaces > 100
and horaire = '22:00';

--14--
select libellegenre, titre from film
left join genre on genre = numgenre;

--15--
select nom from cinema
left join salle on cinema.numcine = salle.numcine
where salle.numcine is null;

--16--
select nom, salle.numsalle, titre, datedeb, datefin, horaire, prix from cinema
left join salle on salle.numcine = cinema.numcine
left join programmation on salle.numcine = programmation.numcine and salle.numsalle = programmation.numsalle
left join film on programmation.numfilm = film.numfilm;

--17--
select sum(sal) from distribution 
where numfilm = (
select numfilm from film 
where titre = 'Jurassic Parc');

--18--
select libellegenre ,count(*) from film 
left join genre on genre = numgenre 
group by libellegenre;

--19--
select libellegenre, sum(budget) from genre 
left join film on numgenre = genre 
where annee between 1990 and 1999 
group by libellegenre 
having sum(budget) > 10000000;

--20--
select nom, ville from cinema, salle 
where cinema.numcine = salle.numcine 
group by nom;

--21--
select nom, pren from personne 
where numpers = any(
select real from film) 
and ville not in(
select ville from cinema);

--22--
select titre, annee, longueur from film 
where longueur >= all(
select longueur from film);

--23--
select titre from film 
left join programmation on film.numfilm = programmation.numfilm 
left join cinema on programmation.numcine = cinema.numcine 
minus 
select titre from film 
left join programmation on film.numfilm = programmation.numfilm 
left join cinema on programmation.numcine = cinema.numcine 
where cinema.nom = 'UGC';

--24--
select nom, sum(sal) from personne 
left join acteur on personne.numpers = acteur.numpers 
left join distribution on distribution.numact = acteur.numact 
group by nom having sum(sal) > any(
select sum(sal_real) from personne 
left join film on numpers = real 
group by nom);

--25--
select nom, sum(sal) from personne 
left join acteur on personne.numpers = acteur.numpers 
left join distribution on distribution.numact = acteur.numact 
group by nom having sum(sal) >= all(
select sum(sal_real) from film 
group by real);

--26--
select distinct nom, pren from personne 
left join film on numpers = real 
left join distribution on distribution.numfilm = film.numfilm 
where sal_real > distribution.sal;