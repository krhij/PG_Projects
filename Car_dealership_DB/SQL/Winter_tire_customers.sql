use CarDealerDB
go
select Person.Name, Person.Surname, Tires.Brand, Tires.Season
from Customer
join Person on Customer.PESEL = Person.PESEL
join Orders on Customer.PESEL = Orders.Customer_PESEL
join Cars on Orders.Order_ID = Cars.Order_ID
join Version on Cars.Version_ID = Version.Version_ID
join Tires on Version.Tires_ID = Tires.Tire_ID
where Tires.Season = 'Summer'