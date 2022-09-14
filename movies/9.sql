SELECT DISTINCT(name) FROM people, movies, stars, ratings
WHERE movies.id = stars.movie_id
AND stars.person_id = people.id
AND ratings.movie_id = movies.id
AND year = 2004
ORDER BY birth ASC;