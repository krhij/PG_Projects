use CarDealerDB
go
select Person.Name, Person.Surname, Salesperson.Commision_percentage/100 as Commision_percentage_percent
from
Salesperson
join Person on Salesperson.PESEL = Person.PESEL
where Salesperson.Commision_percentage > (
select avg(Salesperson.Commision_percentage)
from
Salesperson
where Salesperson.Title = 'Manager'
);