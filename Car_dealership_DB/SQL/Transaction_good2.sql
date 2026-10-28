use CarDealerDB;
set transaction isolation level serializable;

begin transaction;

    select Status from Cars where VIN = 'VIN12345678900021';

    --delay to give time to run the second script
    waitfor delay '00:00:10';

    update Cars set Status = 'Sold' where VIN = 'VIN12345678900021';
    
    print 'Car Sold!';

commit transaction;