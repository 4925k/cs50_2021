-- Keep a log of any SQL queries you execute as you solve the mystery.

--looking at the crime report
SELECT * FROM crime_scene_reports;
-- Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse.
-- Interviews were conducted today with three witnesses who were present at the time —
-- each of their interview transcripts mentions the courthouse.

-- looking at the interviews
SELECT * FROM interviews WHERE day = 28;
-- 1. If you have security footage from the courthouse parking lot,
-- you might want to look for cars that left the parking lot in that time frame.
-- 2. I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.
-- 3. As the thief was leaving the courthouse, they called someone who talked to them for less than a minute.
-- In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
-- The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- looking for car records
SELECT * FROM courthouse_security_logs WHERE day = 28 AND hour = 10;
/*
id | year | month | day | hour | minute | activity | license_plate
260 | 2020 | 7 | 28 | 10 | 16 | exit | 5P2BI95
261 | 2020 | 7 | 28 | 10 | 18 | exit | 94KL13X
262 | 2020 | 7 | 28 | 10 | 18 | exit | 6P58WS2
263 | 2020 | 7 | 28 | 10 | 19 | exit | 4328GD8
264 | 2020 | 7 | 28 | 10 | 20 | exit | G412CB7
265 | 2020 | 7 | 28 | 10 | 21 | exit | L93JTIZ
266 | 2020 | 7 | 28 | 10 | 23 | exit | 322W7JE
267 | 2020 | 7 | 28 | 10 | 23 | exit | 0NTHK55
268 | 2020 | 7 | 28 | 10 | 35 | exit | 1106N58
*/

-- looking for atm transactions
SELECT * FROM atm_transactions WHERE day = 28 AND atm_location = "Fifer Street";
/*
id | account_number | year | month | day | atm_location | transaction_type | amount
246 | 28500762 | 2020 | 7 | 28 | Fifer Street | withdraw | 48
264 | 28296815 | 2020 | 7 | 28 | Fifer Street | withdraw | 20
266 | 76054385 | 2020 | 7 | 28 | Fifer Street | withdraw | 60
267 | 49610011 | 2020 | 7 | 28 | Fifer Street | withdraw | 50
269 | 16153065 | 2020 | 7 | 28 | Fifer Street | withdraw | 80
288 | 25506511 | 2020 | 7 | 28 | Fifer Street | withdraw | 20
313 | 81061156 | 2020 | 7 | 28 | Fifer Street | withdraw | 30
336 | 26013199 | 2020 | 7 | 28 | Fifer Street | withdraw | 35
*/

-- looking for call records
SELECT * FROM phone_calls WHERE day = 28 AND duration < 60;
/*
id | caller | receiver | year | month | day | duration
221 | (130) 555-0289 | (996) 555-8899 | 2020 | 7 | 28 | 51
224 | (499) 555-9472 | (892) 555-8872 | 2020 | 7 | 28 | 36
233 | (367) 555-5533 | (375) 555-8161 | 2020 | 7 | 28 | 45
251 | (499) 555-9472 | (717) 555-1342 | 2020 | 7 | 28 | 50
254 | (286) 555-6063 | (676) 555-6554 | 2020 | 7 | 28 | 43
255 | (770) 555-1861 | (725) 555-3243 | 2020 | 7 | 28 | 49
261 | (031) 555-6622 | (910) 555-3251 | 2020 | 7 | 28 | 38
279 | (826) 555-1652 | (066) 555-9701 | 2020 | 7 | 28 | 55
281 | (338) 555-6650 | (704) 555-2131 | 2020 | 7 | 28 | 54
*/

-- looking at flights
SELECT * FROM flights WHERE day = 29
AND origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville");
/*
id | origin_airport_id | destination_airport_id | year | month | day | hour | minute
18 | 8 | 6 | 2020 | 7 | 29 | 16 | 0
23 | 8 | 11 | 2020 | 7 | 29 | 12 | 15
36 | 8 | 4 | 2020 | 7 | 29 | 8 | 20
43 | 8 | 1 | 2020 | 7 | 29 | 9 | 30
53 | 8 | 9 | 2020 | 7 | 29 | 15 | 20
*/

-- checking earliest flights passengers
SELECT * FROM passengers WHERE flight_id = 36;
/*
flight_id | passport_number | seat
36 | 7214083635 | 2A
36 | 1695452385 | 3B
36 | 5773159633 | 4A
36 | 1540955065 | 5C
36 | 8294398571 | 6C
36 | 1988161715 | 6D
36 | 9878712108 | 7A
36 | 8496433585 | 7B
*/

-- look for person with matching passport number, phone number and plate number from given results
-- do a linear search on passport numbers, try to match phone number and plate number
SELECT * FROM people WHERE passport_number = 5773159633;
/*
id | name | phone_number | passport_number | license_plate
686048 | Ernest | (367) 555-5533 | 5773159633 | 94KL13X
*/

-- find where he escaped to
SELECT city FROM airports WHERE id = 4;
-- London

-- find accomplice from called number
SELECT * FROM people WHERE phone_number = "(375) 555-8161";
/*
id | name | phone_number | passport_number | license_plate
864400 | Berthold | (375) 555-8161 |  | 4V16VO0
*/



SELECT name FROM people where passport_number =
(SELECT passport_number FROM passengers WHERE flight_id=36 AND passport_number IN (
SELECT passport_number FROM people WHERE phone_number IN (
SELECT caller FROM phone_calls WHERE year=2020 AND month=7 AND day=28 AND duration<60 AND caller in
(SELECT phone_number FROM people WHERE id in
(SELECT person_id FROM bank_accounts where account_number IN
(SELECT account_number FROM atm_transactions WHERE year=2020 AND month=7 AND day=28 AND atm_location='Fifer Street'
AND transaction_type='withdraw' AND account_number IN 
(SELECT account_number FROM bank_accounts WHERE person_id IN
(SELECT id FROM people WHERE license_plate IN
(SELECT license_plate FROM courthouse_security_logs WHERE year=2020 AND month=7 AND day=28 AND hour=10AND minute >=15 AND minute <=25 and activity="exit")))))))));