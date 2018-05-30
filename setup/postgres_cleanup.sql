CREATE TABLE IF NOT EXISTS tempToCleanUp (
  userId int NOT NULL,
  teamId int NOT NULL
);

DELETE FROM tempToCleanUp;

INSERT INTO tempToCleanUp
SELECT u.id, u.team
FROM public.users u
WHERE u.name LIKE 'server_%'
	and u.user_type = 2;

DELETE FROM public.users u
WHERE u.id IN (SELECT userId FROM tempToCleanUp);

DELETE FROM public.teams t
WHERE t.id IN (SELECT teamId FROM tempToCleanUp)
AND t.name LIKE 'server_%';

DROP TABLE IF EXISTS tempToCleanUp;