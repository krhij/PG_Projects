USE CarDealerDB;
GO

-- 1. Check if Car ...00006 is now 'Matte Grey'
SELECT VIN, Color, Status 
FROM Cars 
WHERE VIN = 'VIN12345678900006';

-- 2. Check if Customer ...00021 speaks 'English' now
SELECT Person.Name, Person.Surname, Customer.Main_language 
FROM Customer
JOIN Person ON Customer.PESEL = Person.PESEL
WHERE Customer.PESEL = '70010100021';

-- 3. Check Order #16 status (Should be 'Pending' instead of Cancelled)
SELECT Order_ID, Status, Budget 
FROM Orders 
WHERE Order_ID = 16;

-- 4. Check Toyota Corolla Prices (Should be higher than original 90000)
SELECT Version.Base_price, Models.Name AS ModelName 
FROM Version
JOIN Models ON Version.Model_ID = Models.Model_ID
WHERE Models.Name = 'Corolla';


-- 5. Check if Car ...00016 is GONE (Should return empty result)
SELECT * FROM Cars WHERE VIN = 'VIN12345678900016';

-- 6. Check if Producer 'Fiat' is GONE (Should return empty result)
SELECT * FROM Producers WHERE Brand_name = 'Fiat';

-- 7. Check if Fiat Cars are GONE (Cascade test - Should return empty)
SELECT Cars.VIN, Models.Name 
FROM Cars
JOIN Version ON Cars.Version_ID = Version.Version_ID
JOIN Models ON Version.Model_ID = Models.Model_ID
JOIN Producers ON Models.Producer_ID = Producers.Producer_ID
WHERE Producers.Brand_name = 'Fiat';
GO