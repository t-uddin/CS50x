SELECT name FROM people
WHERE id IN (SELECT person_id FROM STARS WHERE movie_id IN (SELECT id FROM movies WHERE title = 'Toy Story'));