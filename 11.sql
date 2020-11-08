SELECT title FROM movies
JOIN ratings ON movies.id = ratings.movie_id
JOIN STARS ON movies.id = stars.movie_id
JOIN people ON people.id = stars.person_id
WHERE people.name = 'Chadwick Boseman'
ORDER BY rating DESC LIMIT 5;