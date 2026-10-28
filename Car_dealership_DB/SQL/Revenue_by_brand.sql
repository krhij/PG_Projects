use CarDealerDB
go
select Master_view.Brand_name, sum(Cars.Price) as Total_revenue
from
Master_view
join Cars on Cars.VIN = Master_view.VIN
where Cars.Status = 'Sold'
group by Master_view.Brand_name
order by Total_revenue desc