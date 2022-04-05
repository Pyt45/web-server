<?php


session_start();

print_r($_POST);
if (isset($_POST['register']))
{
	echo $_POST['username'] . ' ' . $_POST['password'];
	if ($_POST['username'] == 'ayoub' && $_POST['password'] == '123456')
	{
		$_SESSION['utilisateur'] = [
			'username' => $_POST['username'],
			'password' => $_POST['password']
		];
	}
}

if (isset($_POST['logout']))
{
	unset($_SESSION['utilisateur']);
	session_destroy();
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>Document</title>
</head>
<body>
	<?php if (!isset($_SESSION['utilisateur'])) {  ?>
		<form method="POST">
			<h2>Authentication</h2>
			<div>
				<label for="username">Username</label>
				<input type="text" name="username">
			</div>
			<div>
				<label for="password">Password</label>
				<input type="password" name="password">
			</div>
			<button type="submit" name="register">Sign in</button>
		</form>
	<?php } else {  ?>
		<h2>Your are connected</h2>
		<form method="post">
			<button type="submit" name="logout">Sign out</button>
		</form>
	<?php } ?>

</body>
</html>