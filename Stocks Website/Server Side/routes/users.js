const express = require('express');
const router = express.Router();
const bcrypt = require('bcrypt');
const jwt = require('jsonwebtoken');


/* POST register a user. */
router.post("/register", function(req, res, next) {
  const email = req.body.email;
  const password = req.body.password;

  // Verify body
  if (!email || !password) {
    res.status(400).json({ "error": true, "message": "Request body incomplete - email and password needed" })
    return;
  }

  const queryUsers = req.db.from("users").select("*").where("email", "=", email)
  queryUsers
    .then((users) => {
      if (users.length > 0) {
        res.status(409).json({ "error": true, "message": "User already exists!" })
        return;
      }

      // Insert user into DB
      const saltRounds = 10;
      const hash = bcrypt.hashSync(password, saltRounds)
      return req.db.from("users").insert({ email, hash })
    })
    .then (() => {
      res.status(201).json({ "success": true, "message": "User created" })
      return;
    })
});


/* POST login a user. */
router.post("/login", function(req, res, next) {
  const email = req.body.email
  const password = req.body.password

  // Verify body
  if( !email || !password) {
    res.status(400).json({ error: true, message: "Request body incomplete - email and password needed" })
    return;
  }

  const queryUsers = req.db.from("users").select("*").where("email", "=", email)
  queryUsers
    .then((users) => {
      if (users.length == 0) {
        res.status(401).json({ error: true, message: "User does not exist" })
        return;
      }

      // Compare password hashes
      const user = users[0]
      return bcrypt.compare(password, user.hash)
    })
    .then((match) => {
      if (!match) {
        res.status(401).json({ error: true, message: "Passwords do not match" })
        return;
      }

      // Create and return JWT token
      const secretKey = "webcomputing";
      const expires_in = 60 * 60 * 24   // 1 day
      const exp = Date.now() + expires_in * 1000
      const token = jwt.sign({ email, exp }, secretKey)
      res.json({ token_type: "Bearer", token, expires_in })
    })
})

module.exports = router;
