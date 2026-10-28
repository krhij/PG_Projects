USE CarDealerDB;
GO

CREATE TABLE Location (
    Location_ID INT IDENTITY(1,1) NOT NULL PRIMARY KEY,
    Name VARCHAR(50) NOT NULL,
    Type VARCHAR(20) CHECK (Type IN ('Headquarters', 'Showroom', 'Service Center', 'Lot')),
    City VARCHAR(50) NOT NULL,
    Street VARCHAR(50) NOT NULL,
    Number_of_positions INT CHECK (Number_of_positions > 0)
);

CREATE TABLE Producers (
    Producer_ID INT IDENTITY(1,1) NOT NULL PRIMARY KEY,
    Brand_name VARCHAR(50) NOT NULL,
    Origin_country VARCHAR(50) NOT NULL
);

CREATE TABLE Engine (
    Engine_ID INT IDENTITY(1,1) NOT NULL PRIMARY KEY,
    Type VARCHAR(20) NOT NULL,
    Horsepower INT CHECK (Horsepower > 0),
    Fuel_type VARCHAR(20) CHECK (Fuel_type IN ('Petrol', 'Diesel', 'Electric', 'Hybrid'))
);

CREATE TABLE Tires (
    Tire_ID INT IDENTITY(1,1) NOT NULL PRIMARY KEY,
    Size INT CHECK (Size BETWEEN 13 AND 24),
    Brand VARCHAR(50) NOT NULL,
    Season VARCHAR(20) CHECK (Season IN ('Summer', 'Winter', 'All-Season', 'Slick'))
);

CREATE TABLE Trim (
    Trim_ID INT IDENTITY(1,1) NOT NULL PRIMARY KEY,
    Name VARCHAR(50) NOT NULL,
    Material VARCHAR(30) CHECK (Material IN ('Leather', 'Cloth', 'Alcantara', 'Vinyl', 'Mixed')),
    Color VARCHAR(30) NOT NULL
);

CREATE TABLE Person (
    PESEL CHAR(11) NOT NULL CONSTRAINT CHK_PESEL_Numeric CHECK (PESEL NOT LIKE '%[^0-9]%') PRIMARY KEY,
    Name VARCHAR(50) NOT NULL,
    Surname VARCHAR(50) NOT NULL,
    Date_of_birth DATE NOT NULL CONSTRAINT CHK_Adult CHECK (DATEDIFF(year, Date_of_birth, GETDATE()) >= 18)
);

CREATE TABLE Models (
    Model_ID INT IDENTITY(1,1) NOT NULL PRIMARY KEY,
    Producer_ID INT NOT NULL,
    Name VARCHAR(50) NOT NULL,
    Release_date INT CHECK (Release_date BETWEEN 1900 AND 2100),
    CONSTRAINT FK_Models_Producers FOREIGN KEY (Producer_ID) REFERENCES Producers(Producer_ID) ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE Salesperson (
    PESEL CHAR(11) NOT NULL UNIQUE,
    Hire_date DATE NOT NULL,
    Title VARCHAR(30) CHECK (Title IN ('Junior Salesman', 'Senior Salesman', 'Manager', 'Director')),
    Commision_percentage FLOAT CHECK (Commision_percentage BETWEEN 0.00 AND 100.00),
    Work_Location_ID INT NOT NULL,
    CONSTRAINT FK_Salesperson_Person FOREIGN KEY (PESEL) REFERENCES Person(PESEL) ON UPDATE CASCADE ON DELETE CASCADE,
    CONSTRAINT FK_Salesperson_Location FOREIGN KEY (Work_Location_ID) REFERENCES Location(Location_ID) ON UPDATE CASCADE
);

CREATE TABLE Customer (
    PESEL CHAR(11) NOT NULL UNIQUE,
    Registration_date DATE NOT NULL,
    Main_language VARCHAR(30) DEFAULT 'Polish',
    CONSTRAINT FK_Customer_Person FOREIGN KEY (PESEL) REFERENCES Person(PESEL) ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE Version (
    Version_ID INT IDENTITY(1,1) NOT NULL PRIMARY KEY,
    Model_ID INT NOT NULL,
    Engine_ID INT NOT NULL,
    Tires_ID INT NOT NULL,
    Trim_ID INT NOT NULL,
    Base_price FLOAT CHECK (Base_price > 0),
    Horsepower INT CHECK (Horsepower > 0),
    CONSTRAINT FK_Version_Models FOREIGN KEY (Model_ID) REFERENCES Models(Model_ID) ON UPDATE CASCADE ON DELETE CASCADE,
    CONSTRAINT FK_Version_Engine FOREIGN KEY (Engine_ID) REFERENCES Engine(Engine_ID) ON UPDATE CASCADE,
    CONSTRAINT FK_Version_Tires FOREIGN KEY (Tires_ID) REFERENCES Tires(Tire_ID) ON UPDATE CASCADE,
    CONSTRAINT FK_Version_Trim FOREIGN KEY (Trim_ID) REFERENCES Trim(Trim_ID) ON UPDATE CASCADE
);

CREATE TABLE Orders (
    Order_ID INT IDENTITY(1,1) NOT NULL PRIMARY KEY,
    Date DATE NOT NULL DEFAULT GETDATE(),
    Status VARCHAR(20) CHECK (Status IN ('Pending', 'Completed', 'Cancelled', 'Audited')),
    Budget FLOAT CHECK (Budget > 0),
    Customer_PESEL CHAR(11) NOT NULL,
    Salesperson_PESEL CHAR(11) NOT NULL,
    CONSTRAINT FK_Orders_Customer FOREIGN KEY (Customer_PESEL) REFERENCES Customer(PESEL) ON UPDATE NO ACTION,
    CONSTRAINT FK_Orders_Salesperson FOREIGN KEY (Salesperson_PESEL) REFERENCES Salesperson(PESEL) ON UPDATE NO ACTION
);

CREATE TABLE Cars (
    VIN CHAR(17) NOT NULL CONSTRAINT CHK_VIN_Length CHECK (LEN(VIN) = 17) PRIMARY KEY,
    Version_ID INT NOT NULL,
    Color VARCHAR(30) NOT NULL,
    Acquisition_date DATE NOT NULL,
    Status VARCHAR(20) CHECK (Status IN ('Available', 'Reserved', 'Sold', 'Maintenance')),
    Price FLOAT CHECK (Price >= 0),
    Order_ID INT NULL,
    CONSTRAINT FK_Cars_Version FOREIGN KEY (Version_ID) REFERENCES Version(Version_ID) ON UPDATE CASCADE ON DELETE CASCADE,
    CONSTRAINT FK_Cars_Orders FOREIGN KEY (Order_ID) REFERENCES Orders(Order_ID) ON UPDATE NO ACTION
);
GO