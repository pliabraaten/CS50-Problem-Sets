-- In 12.sql, write a SQL query to list the titles of all movies in which both Bradley Cooper and Jennifer Lawrence starred.
-- Your query should output a table with a single column for the title of each movie.
-- You may assume that there is only one person in the database with the name Bradley Cooper.
-- You may assume that there is only one person in the database with the name Jennifer Lawrence.

-- Notice that this query, like the previous, requires you to use data from multiple tables. Recall that you can “nest” queries in SQL, which allows you to break a larger query into smaller ones. Perhaps you could write queries to…

-- Find the ID of Bradley Cooper
-- Find the ID of Jennifer Lawrence
-- Find the IDs of movies associated with Bradley Cooper’s ID
-- Find the IDs of movies associated with Jennifer Lawrence’s ID
-- Find movie titles from the movie IDs associated with both Bradley Cooper and Jennifer Lawrence

-- WHERE column_name IN (subquery1) AND column_name IN (subquery2)

SELECT DISTINCT movies.title FROM movies
JOIN stars ON movies.id = stars.movie_id
WHERE movies.id IN
    (SELECT stars.movie_id FROM stars
    JOIN people ON stars.person_id = people.id
    WHERE people.name = "Bradley Cooper")
AND movies.id IN
    (SELECT stars.movie_id FROM stars
    JOIN people ON stars.person_id = people.id
    WHERE people.name = "Jennifer Lawrence");
