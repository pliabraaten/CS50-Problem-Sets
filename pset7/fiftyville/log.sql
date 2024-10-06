-- Keep a log of any SQL queries you execute as you solve the mystery.

-- All you know is that the theft took place on July 28, 2023 and that it took place on Humphrey Street.

    -- Who the thief is,
    -- What city the thief escaped to, and
    -- Who the thief’s accomplice is who helped them escape

-- Look in the crime_scene_reports table
SELECT description FROM crime_scene_reports
WHERE year = 2023
AND month = 7
AND day = 28
AND street = "Humphrey Street";

-- Result:
    -- Theft at 10:15am at the Humphrey Street bakery
    -- Interviews of three witnesses: interview transcripts mentions the bakery

-- Check interview transcripts for mentions of the bakery
SELECT transcript, name FROM interviews
WHERE transcript LIKE "%bakery%"
AND year = 2023
AND month = 7
AND day = 28;

-- Result:
    -- Ruth: within ten minutes, thief drove away; check security footage from parking lot that left within time range
    -- Eugene: recongized thief withdrawing from ATM on Leggett Street earlier that morning
    -- Raymond: leaving thief called someone for less than a minute; planning to take earliest flight in the morning -> other person on phone purchased flight

-- Pull list of names of car owners in the parking lot that left between 10:15 and 10:25 (ten minutes after theft)
SELECT people.name, bakery_security_logs.minute FROM people
JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
WHERE year = 2023
AND month = 7
AND day = 28
AND hour = 10
AND minute < 25 AND minute > 15
AND bakery_security_logs.activity = "exit"
ORDER BY name;

-- Result: Barry, Bruce, Diana, Iman, Kelsey, Luca, Sofia, Vanessa

-- Pull names of people with ATM transactions for these suspects checking for usage on Leggett Street earlier that morning
SELECT people.name, account_number, passport_number, license_plate FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE bank_accounts.account_number IN
(
    SELECT atm_transactions.account_number FROM atm_transactions
    WHERE year = 2023 AND month = 7 AND day = 28
    AND atm_location = "Leggett Street"
    AND transaction_type = "withdraw"
)
ORDER BY name;

-- Result: crossover: Bruce, Diana, Iman, Luca

-- Check phone calls for callers directly after the theft
SELECT name, duration, receiver FROM people
JOIN phone_calls ON people.phone_number = phone_calls.caller
WHERE year = 2023 AND month = 7 AND day = 28
AND duration < 60
ORDER BY name;

-- Result: crossover: Bruce, Diana

-- Check phone calls for who Diana called directly after the theft
SELECT p1.name AS receiver, p2.name AS caller FROM phone_calls
JOIN people p1 ON phone_calls.receiver = p1.phone_number
JOIN people p2 ON phone_calls.caller = p2.phone_number
WHERE phone_calls.caller IN
    (
        SELECT people.phone_number FROM people
        WHERE name IN ("Bruce", "Diana")
    )
AND year = 2023 AND month = 7 AND day = 28
AND duration < 60;

-- Result: Philip (Diana) or Robin (Bruce)

-- Look for destination of earliest flight booked for Diana for July 29th morning
SELECT city, hour, name FROM airports
JOIN flights ON airports.id = flights.destination_airport_id
JOIN passengers ON flights.id = passengers.flight_id
JOIN people ON passengers.passport_number = people.passport_number
WHERE year = 2023 AND month = 7 AND day = 29
AND passengers.passport_number IN
    (
        SELECT passport_number FROM people
        WHERE people.name IN ("Bruce", "Diana")
    )
ORDER BY hour;


-- Result: New York City, Bruce -> Robin

