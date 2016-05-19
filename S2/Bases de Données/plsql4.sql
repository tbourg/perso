set serveroutput on;

/*
declare
cursor c1 is select ename, comm from emp;
nom emp.ename%type;
comm emp.comm%type;
begin
open c1;
loop
fetch c1 into nom, comm;
exit when c1%notfound;
if comm = 0 or comm is null
then dbms_output.put_line('L''employé '||nom||' n''a pas de commission');
else dbms_output.put_line('L''employé '||nom||' a une commission de '||comm||' dollars');
end if;
end loop;
close c1;
end;*/

/*declare
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
end;*/

create table employees as select * from hr.employees;