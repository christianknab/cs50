-- Keep a log of any SQL queries you execute as you solve the mystery.

-- See date and time and description of the CS50 duck theft
SELECT * FROM crime_scene_reports
WHERE street like "Humphrey Street";
-- | 295 | 2021 | 7  | 28  | Humphrey Street |
-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.

-- Check interviews
SELECT * FROM interviews
WHERE year = 2021 AND month >= 7 AND transcript LIKE "%bakery%";
-- | 161 | Ruth    | 2021 | 7     | 28  | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
-- | 162 | Eugene  | 2021 | 7     | 28  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
-- | 163 | Raymond | 2021 | 7     | 28  | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. |

-- Check security footage for theif's car
SELECT * FROM bakery_security_logs
WHERE year = 2021 AND month =  7 AND day = 28 AND hour BETWEEN 9 AND 10 AND minute BETWEEN 0 AND 30;
-- | 260 | 2021 | 7     | 28  | 10   | 16     | exit     | 5P2BI95       |
-- | 261 | 2021 | 7     | 28  | 10   | 18     | exit     | 94KL13X       |
-- | 262 | 2021 | 7     | 28  | 10   | 18     | exit     | 6P58WS2       |
-- | 263 | 2021 | 7     | 28  | 10   | 19     | exit     | 4328GD8       |
-- | 264 | 2021 | 7     | 28  | 10   | 20     | exit     | G412CB7       |
-- | 265 | 2021 | 7     | 28  | 10   | 21     | exit     | L93JTIZ       |
-- | 266 | 2021 | 7     | 28  | 10   | 23     | exit     | 322W7JE       |
-- | 267 | 2021 | 7     | 28  | 10   | 23     | exit     | 0NTHK55       |

-- Check names of the license plates of people who left between 15 minutes after theft
SELECT * FROM people, bakery_security_logs
WHERE bakery_security_logs.license_plate = people.license_plate
AND people.license_plate IN (SELECT license_plate FROM bakery_security_logs
WHERE minute BETWEEN 15 AND 30 AND hour = 10 AND year = 2021 AND month = 7 AND day = 28)
ORDER BY hour;
-- | 221103 | Vanessa | (725) 555-4692 | 2963008352      | 5P2BI95       | 260 | 2021 | 7     | 28  | 10   | 16     | exit     | 5P2BI95       |
-- | 243696 | Barry   | (301) 555-4174 | 7526138472      | 6P58WS2       | 262 | 2021 | 7     | 28  | 10   | 18     | exit     | 6P58WS2       |
-- | 396669 | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       | 265 | 2021 | 7     | 28  | 10   | 21     | exit     | L93JTIZ       |
-- | 398010 | Sofia   | (130) 555-0289 | 1695452385      | G412CB7       | 264 | 2021 | 7     | 28  | 10   | 20     | exit     | G412CB7       |
-- | 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       | 263 | 2021 | 7     | 28  | 10   | 19     | exit     | 4328GD8       |
-- | 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       | 266 | 2021 | 7     | 28  | 10   | 23     | exit     | 322W7JE       |
-- | 560886 | Kelsey  | (499) 555-9472 | 8294398571      | 0NTHK55       | 267 | 2021 | 7     | 28  | 10   | 23     | exit     | 0NTHK55       |
-- | 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       | 261 | 2021 | 7     | 28  | 10   | 18     | exit     | 94KL13X       |
-- +--------+---------+----------------+-----------------+---------------+-----+------+-------+-----+------+--------+----------+---------------+

-- Check phone calls morning of 2021, 7, 28 between 10:15 - 10:25
SELECT * FROM phone_calls
WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60
AND caller in (SELECT phone_number FROM people, bakery_security_logs
WHERE bakery_security_logs.license_plate = people.license_plate
AND people.license_plate IN (SELECT license_plate FROM bakery_security_logs
WHERE minute BETWEEN 15 AND 30 AND hour = 10 AND year = 2021 AND month = 7 AND day = 28)
ORDER BY hour);
-- +-----+----------------+----------------+------+-------+-----+----------+
-- | id  |     caller     |    receiver    | year | month | day | duration |
-- +-----+----------------+----------------+------+-------+-----+----------+
-- | 221 | (130) 555-0289 | (996) 555-8899 | 2021 | 7     | 28  | 51       |
-- | 224 | (499) 555-9472 | (892) 555-8872 | 2021 | 7     | 28  | 36       |
-- | 233 | (367) 555-5533 | (375) 555-8161 | 2021 | 7     | 28  | 45       |
-- | 251 | (499) 555-9472 | (717) 555-1342 | 2021 | 7     | 28  | 50       |
-- | 255 | (770) 555-1861 | (725) 555-3243 | 2021 | 7     | 28  | 49       |
-- +-----+----------------+----------------+------+-------+-----+----------+

-- CALLED       Sofia, Kelsey, Bruce, Diana

-- Check ATM transactions on Leggett Street in the morning
SELECT * FROM atm_transactions
WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw";
-- +-----+----------------+------+-------+-----+----------------+------------------+--------+
-- | id  | account_number | year | month | day |  atm_location  | transaction_type | amount |
-- +-----+----------------+------+-------+-----+----------------+------------------+--------+
-- | 246 | 28500762       | 2021 | 7     | 28  | Leggett Street | withdraw         | 48     |
-- | 264 | 28296815       | 2021 | 7     | 28  | Leggett Street | withdraw         | 20     |
-- | 266 | 76054385       | 2021 | 7     | 28  | Leggett Street | withdraw         | 60     |
-- | 267 | 49610011       | 2021 | 7     | 28  | Leggett Street | withdraw         | 50     |
-- | 269 | 16153065       | 2021 | 7     | 28  | Leggett Street | withdraw         | 80     |
-- | 288 | 25506511       | 2021 | 7     | 28  | Leggett Street | withdraw         | 20     |
-- | 313 | 81061156       | 2021 | 7     | 28  | Leggett Street | withdraw         | 30     |
-- | 336 | 26013199       | 2021 | 7     | 28  | Leggett Street | withdraw         | 35     |
-- +-----+----------------+------+-------+-----+----------------+------------------+--------+

-- See who withdrew money ^^
SELECT bank_accounts.*, people.name FROM bank_accounts, atm_transactions, people
WHERE bank_accounts.account_number = atm_transactions.account_number
AND people.id = bank_accounts.person_id
AND bank_accounts.account_number IN (SELECT account_number FROM atm_transactions
WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw");
-- +----------------+-----------+---------------+---------+
-- | account_number | person_id | creation_year |  name   |
-- +----------------+-----------+---------------+---------+
-- | 49610011       | 686048    | 2010          | Bruce   |
-- | 49610011       | 686048    | 2010          | Bruce   |
-- | 26013199       | 514354    | 2012          | Diana   |
-- | 26013199       | 514354    | 2012          | Diana   |
-- | 16153065       | 458378    | 2012          | Brooke  |
-- | 28296815       | 395717    | 2014          | Kenny   |
-- | 28296815       | 395717    | 2014          | Kenny   |
-- | 25506511       | 396669    | 2014          | Iman    |
-- | 25506511       | 396669    | 2014          | Iman    |
-- | 28500762       | 467400    | 2014          | Luca    |
-- | 28500762       | 467400    | 2014          | Luca    |
-- | 76054385       | 449774    | 2015          | Taylor  |
-- | 81061156       | 438727    | 2018          | Benista |
-- | 81061156       | 438727    | 2018          | Benista |
-- +----------------+-----------+---------------+---------+

-- BRUCE OR DIANA IS KILLER

-- Flights 2021, 7, 29, earliest flight out of fiftyville
SELECT passengers.*, people.name, flights.hour FROM passengers, flights, airports, people
WHERE airports.id = flights.origin_airport_id
AND flights.id = passengers.flight_id
AND people.passport_number = passengers.passport_number
AND flights.hour < 12
AND city = "Fiftyville" AND year = 2021 AND month = 7 AND day = 29;
-- +-----------+-----------------+------+---------+------+
-- | flight_id | passport_number | seat |  name   | hour |
-- +-----------+-----------------+------+---------+------+
-- | 36        | 7214083635      | 2A   | Doris   | 8    |
-- | 36        | 1695452385      | 3B   | Sofia   | 8    |
-- | 36        | 5773159633      | 4A   | Bruce   | 8    |
-- | 36        | 1540955065      | 5C   | Edward  | 8    |
-- | 36        | 8294398571      | 6C   | Kelsey  | 8    |
-- | 36        | 1988161715      | 6D   | Taylor  | 8    |
-- | 36        | 9878712108      | 7A   | Kenny   | 8    |
-- | 36        | 8496433585      | 7B   | Luca    | 8    |
-- | 43        | 7597790505      | 7B   | Daniel  | 9    |
-- | 43        | 6128131458      | 8A   | Carol   | 9    |
-- | 43        | 6264773605      | 9A   | Rebecca | 9    |
-- | 43        | 3642612721      | 2C   | Sophia  | 9    |
-- | 43        | 4356447308      | 3B   | Heather | 9    |
-- | 43        | 7441135547      | 4A   | Marilyn | 9    |
-- +-----------+-----------------+------+---------+------+

-- BRUCE IS THE KILLER

-- Where the flight is headed
SELECT flights.*, airports.city FROM flights, airports
WHERE flights.id = 36
AND airports.id = flights.destination_airport_id
AND year = 2021 AND month = 7 AND day = 29;
-- +----+-------------------+------------------------+------+-------+-----+------+--------+---------------+
-- | id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |     city      |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+---------------+
-- | 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     | New York City |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+---------------+

-- CITY IS NEW YORK CITY

-- Who recieved Bruce's call?
SELECT * FROM people
WHERE phone_number = "(375) 555-8161";
-- +--------+-------+----------------+-----------------+---------------+
-- | 864400 | Robin | (375) 555-8161 |                 | 4V16VO0       |
-- +--------+-------+----------------+-----------------+---------------+

CREATE TABLE crime_scene_reports (
    id INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    street TEXT,
    description TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE interviews (
    id INTEGER,
    name TEXT,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    transcript TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE atm_transactions (
    id INTEGER,
    account_number INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    atm_location TEXT,
    transaction_type TEXT,
    amount INTEGER,
    PRIMARY KEY(id)
);
CREATE TABLE bank_accounts (
    account_number INTEGER,
    person_id INTEGER,
    creation_year INTEGER,
    FOREIGN KEY(person_id) REFERENCES people(id)
);
CREATE TABLE airports (
    id INTEGER,
    abbreviation TEXT,
    full_name TEXT,
    city TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE flights (
    id INTEGER,
    origin_airport_id INTEGER,
    destination_airport_id INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    hour INTEGER,
    minute INTEGER,
    PRIMARY KEY(id),
    FOREIGN KEY(origin_airport_id) REFERENCES airports(id),
    FOREIGN KEY(destination_airport_id) REFERENCES airports(id)
);
CREATE TABLE passengers (
    flight_id INTEGER,
    passport_number INTEGER,
    seat TEXT,
    FOREIGN KEY(flight_id) REFERENCES flights(id)
);
CREATE TABLE phone_calls (
    id INTEGER,
    caller TEXT,
    receiver TEXT,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    duration INTEGER,
    PRIMARY KEY(id)
);
CREATE TABLE people (
    id INTEGER,
    name TEXT,
    phone_number TEXT,
    passport_number INTEGER,
    license_plate TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE bakery_security_logs (
    id INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    hour INTEGER,
    minute INTEGER,
    activity TEXT,
    license_plate TEXT,
    PRIMARY KEY(id)
);