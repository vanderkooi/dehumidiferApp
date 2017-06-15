exports.up = function(knex, Promise) {
  return knex.schema.createTable('humTemp', function(table) {
    table.increments('id').primary()
    table.integer('humidity').notNullable()
    table.integer('temperature').notNullable()
    table.timestamps(false, true)
  })
};

exports.down = function(knex, Promise) {
  return knex.schema.dropTableIfExists('humTemp')
};

