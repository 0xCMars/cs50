-- Keep a log of any SQL queries you execute as you solve the mystery.
-- find the crime_scene_reports
SELECT * FROM crime_scene_reports
WHERE year = 2020
AND month = 7
AND day = 28
AND street = "Chamberlin Street"
;
-- Theft of the CS50 duck took place at 10:15am
-- at the Chamberlin Street courthouse.
-- Interviews were conducted today with three witnesses
-- who were present at the time — each of their interview transcripts mentions
-- the courthouse.

-- find the interview transcripts mentions the courthouse
SELECT * FROM interviews
WHERE year = 2020
AND month = 7
AND day = 28
AND transcript LIKE "%courthouse%"
;

-- interview id = 161 mention the security footage ,"within 10 minutes"
-- so find the car license_plate which activity in that period
SELECT * FROM courthouse_security_logs
WHERE year = 2020
AND month = 7
AND day = 28
AND hour = 10
AND minute >= 15
AND minute <= 25
;

-- inerview id = 162 mention the atm on Fifer Street and saw the thief withdrawing money
SELECT * FROM atm_transactions
WHERE year = 2020
AND month = 7
AND day = 28
AND transaction_type = "withdraw"
AND atm_location = "Fifer Street"
;

-- interview id = 163 mention the thief were taking the earliest flight out of Fiftyville
-- in July 29
SELECT * FROM flights
WHERE year = 2020
AND month = 7
AND day = 29
AND origin_airport_id =
(
SELECT id FROM airports WHERE city LIKE "Fiftyville"
)
ORDER BY hour
;

-- so we find the earliest flight: id = 36, time = 8:20,
-- so we have find what city the thief escaped to
-- destination_airport_id = 4
SELECT city FROM airports WHERE id = 4;
-- London

-- Next we should find who is withdraw money from atm, take the flight to London
-- ,left the parking lot in that time frame and
-- make a phone call less than a minute
-- That is the suspect we are looking for
SELECT name FROM people
WHERE id IN
(
    SELECT person_id FROM bank_accounts
    WHERE account_number IN
    (
        SELECT account_number FROM atm_transactions
        WHERE year = 2020
        AND month = 7
        AND day = 28
        AND transaction_type = "withdraw"
        AND atm_location = "Fifer Street"
    )
    INTERSECT
    SELECT id FROM people
    WHERE passport_number IN
    (
        SELECT passport_number FROM passengers
        WHERE flight_id IN
        (
            SELECT id FROM flights
            WHERE hour = 8
            AND minute = 20
            AND year = 2020
            AND month = 7
            AND day = 29
            AND origin_airport_id =
            (SELECT id FROM airports WHERE city = "Fiftyville")
        )
    )
    INTERSECT
    SELECT id FROM people
    WHERE license_plate IN
    (
        SELECT license_plate FROM courthouse_security_logs
        WHERE year = 2020
        AND month = 7
        AND day = 28
        AND hour = 10
        AND minute >= 15
        AND minute <= 25
    )
)
AND phone_number IN
(
SELECT caller FROM phone_calls
WHERE year = 2020
AND month = 7
AND day = 28
AND duration < 60
);
-- Only one suspect is meets the above conditions
-- the thief Ernest

-- According to the phone call, we can find the ACCOMPLIC
SELECT name FROM people
WHERE phone_number IN
(
    SELECT receiver FROM phone_calls
    WHERE caller =
    (SELECT phone_number FROM people WHERE name = "Ernest")
    AND year = 2020
    AND month = 7
    AND day = 28
    AND duration < 60
)
;