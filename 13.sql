SELECT name FROM people
WHERE id IN (SELECT person_id FROM stars 
WHERE movie_id IN (SELECT movie_id FROM stars 
WHERE person_id IN (SELECT id FROM people 
WHERE people.name = 'Kevin Bacon' AND birth = 1958)))
AND people.name != 'Kevin Bacon'
ORDER BY name;