SELECT name FROM people, movies, stars
WHERE movies.id = stars.movie_id
AND stars.person_id = people.id
AND title like  "Toy Story";