const express = require('express');
const router = express.Router();
const jwt = require('jsonwebtoken');


/* GET all stocks, with optional industry search */
router.get('/symbols', function(req, res, next) { 
  // If no industry parameter is provided
  if (Object.keys(req.query).length == 0) {
    req.db
    .from("stocks")
    .distinct("name", "symbol", "industry")
    .then((rows) => {
      res.status(200).json(rows)
      return;
    })
  } 

  // If an industry parameter is provided
  else {
      if (req.query.industry) {
        const industry = '%' + req.query.industry + '%'
        req.db
        .from("stocks")
        .distinct("name", "symbol", "industry")
        .where("industry", "like", industry)
        .then((rows) => { 
          if (rows.length == 0) {
            res.status(404).json({ error: true, message: "Industry sector not found" })
            return;
          } 
          
          else {
            res.status(200).json(rows)
            return;
          }
        })
      }
      
      else {
        res.status(400).json({ error: true, message: "Invalid query parameter: only 'industry' is permitted" });
        return;
      }
    }
});


/* GET stocks by symbol */
router.get('/:symbol', function(req, res) {
  if (Object.keys(req.query).length !== 0) {
    res.status(400).json({error : true, message: "No query parameters are permitted" })
    return;
} 

  if (req.params.symbol !== req.params.symbol.toUpperCase() || req.params.symbol.length > 5) {
    res.status(400).json({ error : true, message: "Stock symbol incorrect format - must be 1-5 capital letters" })
    return;
  }

  req.db
  .from("stocks")
  .distinct("*")
  .where("symbol", "=", req.params.symbol)
  .then((rows) => {
    if (rows.length == 0) {
      res.status(404).json({ error: true, message: "No entry for symbol in stocks database" })
      return;
    }

    res.status(200).json(rows[0])
    return;    
  })
  .catch((err) => {
    console.log(err);
    res.json({ error: true, message: "Error executing MySQL query" })
  })
});


// Lambda function for JWT authorization
const authorize = (req, res, next) => {
  const authorization = req.headers.authorization
  let token = null;

  // Retrieve token
  if (authorization && authorization.split(" ").length == 2) {
    token = authorization.split(" ")[1]
  } else {
    // If no token is found
    res.status(403).json({error: true, message: "Unauthorized user"})
    return;
  }

  // Verify JWT and check expiration date
  try {
    const decoded = jwt.verify(token, "webcomputing")

    if (decoded.exp < Date.now()) {
      res.status(403).json({ error: true, message: "Token has expired"})
      return;
    }

    // If token is valid, permit user to advance to route
    next()
  } catch (err) {
    res.status(403).json({error: true, message: "Token is not valid", "Error catched": err})
  }
}


/* GET stocks by symbol through authorised */
router.get("/authed/:symbol", authorize, function(req,res, next){
  const symbol = req.params.symbol;

  if (symbol == symbol.toUpperCase() && (symbol.length > 0 && symbol.length < 6)) {
    req.db
    .from('stocks')
    .select("*")
    .where('symbol', '=', req.params.symbol)
    .then((rows) => {
      if(rows.length == 0) {
        res.status(404).json({ error: true, message: "No entry for symbol in stocks database" })
        return;
      } 

      if (Object.keys(req.query).length == 0) {
        res.status(200).json(rows[0])
        return;
      } 
        
      else {
        if (req.query.from || req.query.to) {
          if (req.query.from && Object.keys(req.query).length == 1) {
            req.db
            .from('stocks')
            .select('*')
            .where('timestamp', '>', req.query.from)
            .where('symbol', '<=', req.params.symbol)
            .then((rows) => {
              if (rows.length == 0) {
                res.status(404).json({ error: true, message: "No entries available for query symbol for supplied date range" })
                return;
              }

              res.status(200).json(rows)
            })
            return;
          }

          else if (req.query.to && Object.keys(req.query).length == 1) {
            req.db
            .from('stocks')
            .select('*')
            .where('timestamp', '<=', req.query.to)
            .where('symbol', '=', req.params.symbol)
            .then((rows) => {
              if (rows.length == 0) {
                res.status(404).json({ error: true, message: "No entries available for query symbol for supplied date range" })
                return;
              }

              res.status(200).json(rows)
            })
            return;
          } 

          else if (Object.keys(req.query).length == 2 && (req.query.to && req.query.from)) {
            if (req.query.from > req.query.to) {
              res.status(404).json({ error: true, message: "No entries available for query symbol for supplied date range" })
              return;
            } 

            req.db
            .from('stocks')
            .select('*')
            .where('timestamp', '>=', req.query.from)
            .where('timestamp', '<=', req.query.to)
            .where('symbol', '=', req.params.symbol)
            .then((rows) => {
              if (rows.length == 0) {
                res.status(404).json({ error: true, message: "No entries available for query symbol for supplied date range" })
                return;
              }

              res.status(200).json(rows)
            })
            return;
          }
            
          else {
            res.status(400).json({ error: true, message: "Parameters allowed are 'from' and 'to', example: /stocks/authed/AAL?from=2020-03-15" })
            return;
          } 
        } 

        else {
          res.status(400).json({ error: true, message: "Parameters allowed are 'from' and 'to', example: /stocks/authed/AAL?from=2020-03-15" })
          return;
        }
      }
    })
  } else {
    res.status(400).json({ error: true, message: "Stock symbol incorrect format - must be 1-5 capital letters" })
    return;
  }
});

module.exports = router;
