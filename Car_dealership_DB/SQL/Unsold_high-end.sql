use CarDealerDB
go
select Models.Name, Producers.Brand_name, Version.Base_price
from
Version
join Models on Version.Model_ID = Models.Model_ID
join Producers on Models.Producer_ID = Producers.Producer_ID
where Version.Base_price > 100000
and Version.Version_ID not in (
select distinct Cars.Version_ID
from
Cars
where Cars.Order_ID is not null
);