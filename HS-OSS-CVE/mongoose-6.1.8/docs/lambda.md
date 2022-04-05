# Using Mongoose With AWS Lambda

[AWS Lambda](https://aws.amazon.com/lambda/) is a popular service for running
arbitrary functions without managing individual servers. Using Mongoose in your
AWS Lambda functions is easy. Here's a sample function that connects to a
MongoDB instance and finds a single document:

```javascript
const mongoose = require('mongoose');

let conn = null;

const uri = 'YOUR CONNECTION STRING HERE';

exports.handler = async function(event, context) {
  // Make sure to add this so you can re-use `conn` between function calls.
  // See https://www.mongodb.com/blog/post/serverless-development-with-nodejs-aws-lambda-mongodb-atlas
  context.callbackWaitsForEmptyEventLoop = false;

  // Because `conn` is in the global scope, Lambda may retain it between
  // function calls thanks to `callbackWaitsForEmptyEventLoop`.
  // This means your Lambda function doesn't have to go through the
  // potentially expensive process of connecting to MongoDB every time.
  if (conn == null) {
    conn = mongoose.createConnection(uri, {
      // and tell the MongoDB driver to not wait more than 5 seconds
      // before erroring out if it isn't connected
      serverSelectionTimeoutMS: 5000
    });
    
    // `await`ing connection after assigning to the `conn` variable
    // to avoid multiple function calls creating new connections
    await conn;
    conn.model('Test', new mongoose.Schema({ name: String }));
  }

  const M = conn.model('Test');

  const doc = await M.findOne();
  console.log(doc);

  return doc;
};
```

## Connection Helper

The above code works fine for a single Lambda function, but what if you want to reuse the same connection logic in multiple Lambda functions?
You can export the below function.

```javascript
'use strict';

const mongoose = require('mongoose');

let conn = null;

const uri = 'YOUR CONNECTION STRING HERE';

exports.connect = async function() {
  if (conn == null) {
    conn = mongoose.createConnection(uri, {
      serverSelectionTimeoutMS: 5000
    });
    
    // `await`ing connection after assigning to the `conn` variable
    // to avoid multiple function calls creating new connections
    await conn;
  }

  return conn;
};
```

## Using `mongoose.connect()`

You can also use `mongoose.connect()`, so you can use `mongoose.model()` to create models.

```javascript
'use strict';

const mongoose = require('mongoose');

let conn = null;

const uri = 'YOUR CONNECTION STRING HERE';

exports.connect = async function() {
  if (conn == null) {
    conn = mongoose.connect(uri, {
      serverSelectionTimeoutMS: 5000
    }).then(() => mongoose);
    
    // `await`ing connection after assigning to the `conn` variable
    // to avoid multiple function calls creating new connections
    await conn;
  }

  return conn;
};
```

*Want to learn how to check whether your favorite JavaScript frameworks, like [Express](http://expressjs.com/) or [React](https://reactjs.org/), work with async/await? Spoiler alert: neither Express nor React support async/await. Chapter 4 of Mastering Async/Await explains the basic principles for determining whether a framework supports async/await. [Get your copy!](http://asyncawait.net/?utm_source=mongoosejs&utm_campaign=lambda)*

<a href="http://asyncawait.net/?utm_source=mongoosejs&utm_campaign=lambda" style="margin-left: 100px">
  <img src="/docs/images/asyncawait.png" style="width: 650px" />
</a>
