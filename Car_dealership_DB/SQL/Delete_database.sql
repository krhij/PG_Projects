/* FORCE DELETE SCRIPT */
USE master;
GO

/* 1. Kick everyone out (Force Disconnect) */
IF EXISTS (SELECT name FROM sys.databases WHERE name = 'CarDealerDB')
BEGIN
    ALTER DATABASE CarDealerDB SET SINGLE_USER WITH ROLLBACK IMMEDIATE;
    DROP DATABASE CarDealerDB;
END
GO

/* 2. Recreate fresh */
CREATE DATABASE CarDealerDB;
GO

USE CarDealerDB;
GO