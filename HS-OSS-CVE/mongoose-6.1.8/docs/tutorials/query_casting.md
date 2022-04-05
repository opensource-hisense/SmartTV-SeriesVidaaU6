# Query Casting

The first parameter to [`Model.find()`](https://mongoosejs.com/docs/api.html#model_Model.find), [`Query#find()`](https://mongoosejs.com/docs/api.html#query_Query-find), [`Model.findOne()`](https://mongoosejs.com/docs/api.html#model_Model.findOne), etc. is called `filter`. In older content this parameter is sometimes called `query` or `conditions`. For example:

```javascript
[require:Cast Tutorial.*get and set]
```

When you execute the query using [`Query#exec()`](https://mongoosejs.com/docs/api.html#query_Query-exec) or [`Query#then()`](https://mongoosejs.com/docs/api.html#query_Query-then), Mongoose _casts_ the filter to match your schema.

```javascript
[require:Cast Tutorial.*cast values]
```

If Mongoose fails to cast the filter to your schema, your query will throw a `CastError`.

```javascript
[require:Cast Tutorial.*cast error]
```

The `strictQuery` Option
------------------------

By default, Mongoose does **not** cast filter properties that aren't in your schema.

```javascript
[require:Cast Tutorial.*not in schema]
```

You can configure this behavior using the [`strictQuery` option for schemas](https://mongoosejs.com/docs/guide.html#strictQuery). This option is analogous to the [`strict` option](https://mongoosejs.com/docs/guide.html#strict). Setting `strictQuery` to `true` removes non-schema properties from the filter:

```javascript
[require:Cast Tutorial.*strictQuery true]
```

To make Mongoose throw an error if your `filter` has a property that isn't in the schema, set `strictQuery` to `'throw'`:

```javascript
[require:Cast Tutorial.*strictQuery throw]
```

Implicit `$in`
--------------

Because of schemas, Mongoose knows what types fields should be, so it can provide some neat syntactic sugar. For example, if you forget to put [`$in`](https://docs.mongodb.com/manual/reference/operator/query/in/) on a non-array field, Mongoose will add `$in` for you.

```javascript
[require:Cast Tutorial.*implicit in]
```