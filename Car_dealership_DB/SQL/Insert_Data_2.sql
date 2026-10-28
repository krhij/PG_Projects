USE CarDealerDB;
GO

INSERT INTO Location (Name, Type, City, Street, Number_of_positions) VALUES
('Warszawa HQ', 'Headquarters', 'Warszawa', 'Aleje Jerozolimskie 100', 50),
('Kraków Showroom', 'Showroom', 'Kraków', 'Wielicka 44', 15),
('Poznañ Service', 'Service Center', 'Poznañ', 'G³ogowska 22', 20),
('Gdañsk Lot', 'Lot', 'Gdañsk', 'Grunwaldzka 10', 100),
('Wroc³aw West', 'Showroom', 'Wroc³aw', 'Legnicka 5', 12),
('£ódŸ Centrum', 'Showroom', '£ódŸ', 'Piotrkowska 1', 10),
('Szczecin North', 'Lot', 'Szczecin', 'Mieszka I 20', 80),
('Lublin East', 'Service Center', 'Lublin', 'Witosa 15', 18),
('Bia³ystok Auto', 'Showroom', 'Bia³ystok', 'Kawaleryjska 9', 10),
('Katowice South', 'Showroom', 'Katowice', 'Chorzowska 50', 15);

INSERT INTO Producers (Brand_name, Origin_country) VALUES
('Toyota', 'Japan'), ('Volkswagen', 'Germany'), ('Ford', 'USA'),
('BMW', 'Germany'), ('Mercedes-Benz', 'Germany'), ('Honda', 'Japan'),
('Hyundai', 'South Korea'), ('Volvo', 'Sweden'), ('Fiat', 'Italy'), ('Mazda', 'Japan');

INSERT INTO Engine (Type, Horsepower, Fuel_type) VALUES
('1.0 TSI', 110, 'Petrol'), ('2.0 TDI', 150, 'Diesel'), ('Hybrid 1.8', 122, 'Hybrid'),
('V6 3.0', 300, 'Petrol'), ('Electric Drive', 204, 'Electric'), ('1.5 EcoBoost', 150, 'Petrol'),
('2.0 SkyActiv-G', 165, 'Petrol'), ('4.0 V8 Biturbo', 500, 'Petrol'), ('1.6 CRDi', 136, 'Diesel'), ('PHEV 2.5', 225, 'Hybrid');

INSERT INTO Tires (Size, Brand, Season) VALUES
(16, 'Michelin', 'Summer'), (17, 'Continental', 'All-Season'), (18, 'Pirelli', 'Summer'),
(19, 'Bridgestone', 'Winter'), (15, 'Debica', 'Winter'), (20, 'Michelin', 'Slick'),
(16, 'Goodyear', 'All-Season'), (21, 'Pirelli', 'Summer'), (17, 'Nokian', 'Winter'), (18, 'Dunlop', 'Summer');

INSERT INTO Trim (Name, Material, Color) VALUES
('Standard', 'Cloth', 'Black'), ('Comfort', 'Cloth', 'Grey'), ('Sport', 'Alcantara', 'Red'),
('Luxury', 'Leather', 'Beige'), ('Executive', 'Leather', 'Black'), ('Night Edition', 'Vinyl', 'Black'),
('R-Design', 'Mixed', 'Black'), ('Family', 'Cloth', 'Blue'), ('Eco', 'Mixed', 'Green'), ('Platinum', 'Leather', 'White');

INSERT INTO Person (PESEL, Name, Surname, Date_of_birth) VALUES
('80010100001', 'Jan', 'Kowalski', '1980-01-01'), ('85020200002', 'Anna', 'Nowak', '1985-02-02'),
('90030300003', 'Piotr', 'Wiœniewski', '1990-03-03'), ('92040400004', 'Maria', 'Wójcik', '1992-04-04'),
('78050500005', 'Krzysztof', 'Kowalczyk', '1978-05-05'), ('88060600006', 'Agnieszka', 'Kamiñska', '1988-06-06'),
('95070700007', 'Micha³', 'Lewandowski', '1995-07-07'), ('82080800008', 'Katarzyna', 'Zieliñska', '1982-08-08'),
('91090900009', 'Tomasz', 'Szymañski', '1991-09-09'), ('89101000010', 'Magdalena', 'WoŸniak', '1989-10-10'),
('83111100011', 'Pawe³', 'D¹browski', '1983-11-11'), ('94121200012', 'Monika', 'Koz³owska', '1994-12-12'),
('86011300013', 'Marcin', 'Jankowski', '1986-01-13'), ('93021400014', 'Ewa', 'Mazur', '1993-02-14'),
('81031500015', 'Jakub', 'Kwiatkowski', '1981-03-15'), ('96041600016', 'Aleksandra', 'Krawczyk', '1996-04-16'),
('87051700017', 'Wojciech', 'Piotrowski', '1987-05-17'), ('90061800018', 'Barbara', 'Grabowska', '1990-06-18'),
('84071900019', 'Marek', 'Paw³owski', '1984-07-19'), ('97082000020', 'Natalia', 'Michalska', '1997-08-20'),
('70010100021', 'Andrzej', 'Król', '1970-01-01'), ('75020200022', 'Dorota', 'Wieczorek', '1975-02-02'),
('60030300023', 'Józef', 'Jab³oñski', '1960-03-03'), ('99040400024', 'Karolina', 'Wróbel', '1999-04-04'),
('85050500025', 'Rafa³', 'Nowakowski', '1985-05-05'), ('91060600026', 'Marta', 'Majewska', '1991-06-06'),
('82070700027', 'Szymon', 'Olszewski', '1982-07-07'), ('93080800028', 'Patrycja', 'Stêpieñ', '1993-08-08'),
('78090900029', '£ukasz', 'Malinowski', '1978-09-09'), ('89101000030', 'Joanna', 'Jaworska', '1989-10-10'),
('65111100031', 'Adam', 'Adamczyk', '1965-11-11'), ('01121200032', 'Wiktoria', 'Dudek', '2001-12-12'),
('73011300033', 'Kamil', 'Górski', '1973-01-13'), ('88021400034', 'Izabela', 'Witkowska', '1988-02-14'),
('95031500035', 'Mateusz', 'Walczak', '1995-03-15'), ('84041600036', 'Paulina', 'Sikora', '1984-04-16'),
('77051700037', 'Grzegorz', 'Baran', '1977-05-17'), ('92061800038', 'Sylwia', 'Rutkowska', '1992-06-18'),
('81071900039', 'Dawid', 'Michalak', '1981-07-19'), ('98082000040', 'Alicja', 'Szewczyk', '1998-08-20'),
('85020212345', 'Jan', 'Nowak', '1985-02-02'),
('80040412345', 'Maria', 'Zielinska', '1980-04-04'),
('90010112345', 'Anna', 'Kowalska', '1990-01-01'),
('99030312345', 'Piotr', 'Wisniewski', '1999-03-03'),
('95050512345', 'Adam', 'Wojcik', '1995-05-05');

INSERT INTO Models (Producer_ID, Name, Release_date) VALUES
(1, 'Corolla', 2023), (1, 'Yaris', 2022), (2, 'Golf', 2023), (2, 'Passat', 2021),
(3, 'Focus', 2022), (3, 'Mustang', 2024), (4, '3 Series', 2023), (5, 'C-Class', 2023),
(6, 'Civic', 2022), (7, 'Tucson', 2023), (8, 'XC60', 2022), (9, '500', 2021),
(10, 'CX-5', 2023), (1, 'RAV4', 2023), (2, 'Tiguan', 2022), (3, 'Kuga', 2023),
(4, 'X5', 2023), (5, 'E-Class', 2024), (6, 'CR-V', 2023), (7, 'i30', 2022),
(4, 'M5', 2024); /* added for the unsold high end query */

INSERT INTO Salesperson (PESEL, Hire_date, Title, Commision_percentage, Work_Location_ID) VALUES
('80010100001', '2010-05-01', 'Manager', 5.0, 1), ('85020200002', '2015-03-15', 'Senior Salesman', 3.5, 1),
('90030300003', '2018-07-20', 'Junior Salesman', 1.5, 2), ('92040400004', '2020-01-10', 'Junior Salesman', 1.5, 2),
('78050500005', '2005-09-01', 'Director', 10.0, 3), ('88060600006', '2012-11-11', 'Senior Salesman', 3.0, 3),
('95070700007', '2021-06-01', 'Junior Salesman', 1.0, 4), ('82080800008', '2008-04-15', 'Manager', 4.5, 4),
('91090900009', '2019-02-28', 'Senior Salesman', 2.5, 5), ('89101000010', '2014-08-10', 'Senior Salesman', 2.5, 5),
('83111100011', '2009-12-05', 'Manager', 4.0, 6), ('94121200012', '2022-03-01', 'Junior Salesman', 1.0, 6),
('86011300013', '2011-06-20', 'Senior Salesman', 3.2, 7), ('93021400014', '2017-09-15', 'Junior Salesman', 1.8, 7),
('81031500015', '2006-01-30', 'Director', 9.0, 8), ('96041600016', '2023-01-10', 'Junior Salesman', 1.0, 8),
('87051700017', '2013-05-25', 'Senior Salesman', 3.0, 9), ('90061800018', '2016-10-10', 'Senior Salesman', 2.8, 9),
('84071900019', '2007-07-07', 'Manager', 4.8, 10), ('97082000020', '2023-05-01', 'Junior Salesman', 1.0, 10),
/* more people for the best performers query */
('85020212345', '2018-03-15', 'Senior Salesman', 15.00, 2), /* Jan: high comission */
('80040412345', '2010-01-01', 'Manager', 12.00, 1), /* Maria: manager baseline */
('90010112345', '2015-06-01', 'Manager', 10.00, 1), /* Anna: manager baseline */
('99030312345', '2023-01-01', 'Junior Salesman', 5.00, 2); /* Piotr: junior */

INSERT INTO Customer (PESEL, Registration_date, Main_language) VALUES
('70010100021', '2023-01-15', 'Polish'), ('75020200022', '2023-02-20', 'Polish'),
('60030300023', '2023-03-05', 'English'), ('99040400024', '2023-04-10', 'Polish'),
('85050500025', '2023-05-15', 'Polish'), ('91060600026', '2023-06-01', 'German'),
('82070700027', '2023-06-20', 'Polish'), ('93080800028', '2023-07-10', 'English'),
('78090900029', '2023-08-05', 'Polish'), ('89101000030', '2023-08-25', 'Polish'),
('65111100031', '2023-09-01', 'Polish'), ('01121200032', '2023-09-15', 'Polish'),
('73011300033', '2023-10-05', 'Polish'), ('88021400034', '2023-10-20', 'English'),
('95031500035', '2023-11-01', 'Polish'), ('84041600036', '2023-11-15', 'Polish'),
('77051700037', '2023-12-01', 'Polish'), ('92061800038', '2023-12-10', 'German'),
('81071900039', '2023-12-20', 'Polish'), ('98082000040', '2023-12-28', 'Polish'),
('95050512345', '2023-01-10', 'Polish'); /* for winter tire query */

INSERT INTO Version (Model_ID, Engine_ID, Tires_ID, Trim_ID, Base_price, Horsepower) VALUES
(1, 3, 1, 2, 90000, 122), (2, 1, 5, 1, 70000, 110), (3, 2, 2, 2, 110000, 150),
(4, 2, 2, 5, 140000, 150), (5, 6, 3, 3, 105000, 150), (6, 8, 8, 4, 250000, 500),
(7, 7, 4, 5, 190000, 165), (8, 2, 4, 5, 210000, 150), (9, 1, 1, 2, 100000, 110),
(10, 3, 7, 2, 130000, 122), (11, 10, 2, 4, 230000, 225), (12, 1, 5, 6, 60000, 110),
(13, 7, 7, 5, 150000, 165), (14, 3, 2, 2, 160000, 122), (15, 2, 9, 2, 145000, 150),
(16, 10, 2, 5, 170000, 225), (17, 4, 8, 4, 350000, 300), (18, 2, 4, 5, 260000, 150),
(19, 3, 1, 5, 155000, 122), (20, 1, 1, 1, 85000, 110),
(21, 2, 1, 2, 150000, 530); /* M5 version - high price for unsold high end query */

INSERT INTO Orders (Date, Status, Budget, Customer_PESEL, Salesperson_PESEL) VALUES
('2023-02-01', 'Completed', 95000, '70010100021', '80010100001'),
('2023-03-01', 'Completed', 75000, '75020200022', '85020200002'),
('2023-04-05', 'Completed', 120000, '60030300023', '90030300003'),
('2023-05-10', 'Pending',   150000, '99040400024', '92040400004'),
('2023-06-01', 'Completed', 260000, '85050500025', '78050500005'),
('2023-06-15', 'Cancelled', 100000, '91060600026', '88060600006'),
('2023-07-01', 'Completed', 370000, '82070700027', '95070700007'),
('2023-07-20', 'Completed', 110000, '93080800028', '82080800008'),
('2023-08-10', 'Pending',   220000, '78090900029', '91090900009'),
('2023-09-01', 'Completed', 140000, '89101000030', '89101000010'),
('2023-09-15', 'Audited',   240000, '65111100031', '83111100011'),
('2023-10-01', 'Completed', 65000, '01121200032', '94121200012'),
('2023-10-10', 'Completed', 160000, '73011300033', '86011300013'),
('2023-11-01', 'Pending',   170000, '88021400034', '93021400014'),
('2023-11-10', 'Completed', 200000, '95031500035', '81031500015'),
('2023-11-20', 'Cancelled', 150000, '84041600036', '96041600016'),
('2023-12-01', 'Completed', 180000, '77051700037', '87051700017'),
('2023-12-10', 'Pending',   270000, '92061800038', '90061800018'),
('2023-12-20', 'Completed', 165000, '81071900039', '84071900019'),
('2023-12-29', 'Completed', 90000, '98082000040', '97082000020'),
/* more orders to put Jan and Anna on the sales leaderboard and test winter tire query*/
('2023-01-15', 'Completed', 40000, '95050512345', '85020212345'),
('2023-03-10', 'Completed', 60000, '95050512345', '85020212345'),
('2023-02-20', 'Completed', 100000, '95050512345', '90010112345'),
('2023-06-15', 'Completed', 155000, '95050512345', '85020212345');

INSERT INTO Cars (VIN, Version_ID, Color, Acquisition_date, Status, Price, Order_ID) VALUES
('VIN12345678900001', 1, 'White', '2023-01-01', 'Sold', 92000, 1),
('VIN12345678900002', 2, 'Red', '2023-01-10', 'Sold', 71000, 2),
('VIN12345678900003', 3, 'Black', '2023-02-01', 'Sold', 112000, 3),
('VIN12345678900004', 4, 'Silver', '2023-02-15', 'Reserved', 142000, 4),
('VIN12345678900005', 6, 'Blue', '2023-03-01', 'Sold', 255000, 5),
('VIN12345678900006', 9, 'Grey', '2023-03-10', 'Available', 100000, NULL),
('VIN12345678900007', 17, 'Black', '2023-04-01', 'Sold', 360000, 7),
('VIN12345678900008', 5, 'White', '2023-05-01', 'Sold', 106000, 8),
('VIN12345678900009', 8, 'Black', '2023-05-15', 'Reserved', 215000, 9),
('VIN12345678900010', 10, 'Blue', '2023-06-01', 'Sold', 132000, 10),
('VIN12345678900011', 11, 'White', '2023-06-10', 'Sold', 235000, 11),
('VIN12345678900012', 12, 'Red', '2023-07-01', 'Sold', 61000, 12),
('VIN12345678900013', 13, 'Grey', '2023-07-15', 'Sold', 152000, 13),
('VIN12345678900014', 14, 'Silver', '2023-08-01', 'Reserved', 162000, 14),
('VIN12345678900015', 7, 'Black', '2023-08-10', 'Sold', 195000, 15),
('VIN12345678900016', 15, 'White', '2023-09-01', 'Available', 145000, NULL),
('VIN12345678900017', 16, 'Blue', '2023-09-15', 'Sold', 175000, 17),
('VIN12345678900018', 18, 'Black', '2023-10-01', 'Reserved', 265000, 18),
('VIN12345678900019', 19, 'Grey', '2023-10-15', 'Sold', 158000, 19),
('VIN12345678900020', 20, 'White', '2023-11-01', 'Sold', 86000, 20),
/* specific cars to test the inventory and sales queries */
('VIN12345678900021', 17, 'Silver', '2023-03-01', 'Available', 120000, NULL), /* X5 - premium stock */
('VIN12345678900022', 21, 'Matt Black', '2023-04-01', 'Available', 155000, NULL), /* M5 - unsold high end */
('VIN12345678900023', 21, 'Green', '2023-04-10', 'Maintenance', 155000, NULL), /* M5 - unsold high end */
('VIN12345678900024', 1, 'White', '2023-01-15', 'Sold', 90000, 21),
('VIN12345678900025', 1, 'Red', '2023-03-10', 'Sold', 90000, 22),
('VIN12345678900026', 4, 'Silver', '2023-02-20', 'Sold', 140000, 23),
('VIN12345678900027', 21, 'Blue', '2023-06-15', 'Sold', 155000, 24);
GO