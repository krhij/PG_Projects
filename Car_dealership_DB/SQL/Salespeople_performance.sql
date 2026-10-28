use CarDealerDB
go
select Person.Name, Person.Surname, count(Orders.Order_ID) as Total_orders, 
sum(Cars.Price) as Total_revenue, sum(Cars.Price * Salesperson.Commision_percentage * 0.01) as Income
from
Salesperson
join Person on Salesperson.PESEL = Person.PESEL
left join Orders on Salesperson.PESEL = Orders.Salesperson_PESEL
left join Cars on Orders.Order_ID = Cars.Order_ID
group by Person.Name, Person.Surname
order by Total_revenue desc;