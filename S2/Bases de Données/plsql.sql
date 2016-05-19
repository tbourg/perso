--Question 1
set serveroutput on;

declare
n number;
moy number;
maj number;
sal number;
n_sal number;
begin 
n := &n;
select avg(sal) into moy from emp group by job having job = (select job from emp where empno = n);
dbms_output.put_line(moy);
select sal*1.1 into maj from emp where empno = n;
select sal into sal from emp where empno = n;
if sal<moy
then n_sal := moy;
else n_sal := maj;
end if;
update emp set sal = n_sal where empno = n; 
end;

--Question 2

set serveroutput on;

declare
n number;
fact number := 1;
begin
n := &n;
for i in 1..n
loop fact := fact*i;
end loop;
dbms_output.put_line('fact('||n||') = '||fact);
end;

--Question 3
--Solution alternative

set serveroutput on;

declare
n number(10);
begin
n := &n;
dbms_output.put_line('col1    col2');
for i in 1..10 
loop dbms_output.put_line(i||'    '||i*n);
end loop;
end;

--Solution avec le tableau

set serveroutput on;

declare
type nom_type is varray(10) of number(3);
col1 nom_type := nom_type(0,0,0,0,0,0,0,0,0,0);
col2 nom_type := nom_type(0,0,0,0,0,0,0,0,0,0);
multip integer;
begin
multip := &multip;
for i in 1..10 loop
col1(i) := i;
col2(i) := i*multip;
DBMS_OUTPUT.PUT_LINE(col1(i)||' * '||multip||' = '||col2(i));
end loop;
end;

--Question 4

alter table dept add budget number(10,2);

set serveroutput on;

declare
cursor d1 is select deptno from dept;
begin
for n in d1
loop update dept set budget = (select sum(sal) from emp group by deptno having deptno = n.deptno) where deptno = n.deptno;
end loop;
end;

set serveroutput on;

declare
cursor e1 is select empno from emp where deptno in (select deptno from dept where loc in ('DALLAS', 'NEW YORK'));
begin
for n in e1
loop update emp set sal = 1.1*sal where empno = n.empno;
end loop;
end;

set serveroutput on;

declare
cursor s1 is select sal from emp order by sal desc;
n number;
cpt number := 0;
begin
n := &n;
for i in s1
loop
exit when cpt = n;
dbms_output.put_line(i.sal);
cpt := cpt + 1;
end loop;
end;
