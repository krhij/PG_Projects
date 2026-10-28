 use CarDealerDB
go
select Models.Name, Producers.Brand_name
from
Cars
join Version on Cars.Version_ID = Version.Version_ID
join Models on Version.Model_ID = Models.Model_ID
join Producers on Models.Producer_ID = Producers.Producer_ID
where Cars.Status = 'Available'
and Version.Base_price > (
select AVG(Version.Base_price)
from
Cars
join Version on Cars.Version_ID = Version.Version_ID
where Cars.Status = 'Available'
);