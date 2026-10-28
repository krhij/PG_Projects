USE CarDealerDB;
GO

-- SCENARIO 1: Delete a single car
DELETE FROM Cars WHERE VIN = 'VIN12345678900016';

-- SCENARIO 2: Delete an entire producer (in case the dealership ceased working with them)
DELETE FROM Producers WHERE Brand_name = 'Fiat';

GO