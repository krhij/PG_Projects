USE CarDealerDB;
GO

-- SCENARIO 1: A car was painted from Grey to 'Matte Grey'
UPDATE Cars
SET Color = 'Matte Grey'
WHERE VIN = 'VIN12345678900006';

-- SCENARIO 2: Customer 'Andrzej Król' (PESEL ...00021) learned English
UPDATE Customer
SET Main_language = 'English'
WHERE PESEL = '70010100021';

-- SCENARIO 3: Raise the commission for all senior salesman by 0.5%
UPDATE Salesperson
SET Commision_percentage = Commision_percentage + 0.5
WHERE Title = 'Senior Salesman';

-- SCENARIO 4: Order #16 was cancelled, but the customer changed their mind. Set to 'Pending'.
UPDATE Orders
SET Status = 'Pending'
WHERE Order_ID = 16;

-- SCENARIO 5: Increase the Base Price of all 'Toyota Corolla' versions (Model_ID = 1) by 10%.
UPDATE Version
SET Base_price = Base_price * 1.10
WHERE Model_ID = 1;

GO