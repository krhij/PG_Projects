use CarDealerDB
go
/* go keyword to make select at the bottom run */
create or alter view Master_view as 
/* create or alter to avoid
"There is already an object named 'Master_view' in the database." error */
select Cars.VIN, Producers.Brand_name, Models.Name, Cars.Color, Cars.Price
from Cars
join Version on Cars.Version_ID = Version.Version_ID
join Models on Version.Model_ID = Models.Model_ID
join Producers on Models.Producer_ID = Producers.Producer_ID;
go

select * from Master_view