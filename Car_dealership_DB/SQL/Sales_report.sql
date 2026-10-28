use CarDealerDB
go
select count(Orders.Order_ID) as Orders, sum(Cars.Price) as Revenue, 
year(Orders.Date) as Sales_year, month(Orders.Date) as Sales_month
from
Orders
join Cars on Orders.Order_ID = Cars.Order_ID
group by year(Orders.Date), month(Orders.Date)
order by Sales_year, Sales_month