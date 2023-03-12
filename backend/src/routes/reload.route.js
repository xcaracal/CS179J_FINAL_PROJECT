const express = require('express');
const Joi = require('joi');
const router = express.Router();
const handle = require('../scripts/schema_handle.script');
const create_header = require('../scripts/create_header.script');
const game_controller = require('../controllers/game.controller');
const player_controller = require('../controllers/player.controller');

const schema = Joi.object().keys({
  id: Joi.number().required()
});

router.post('/', (request, response) => {
  console.log('POST /reload')
  const { error, value } = handle(request, response, schema);
  if (error) return;

  const id = value.id;
  const player = game_controller.getPlayer(id);

  // Check if the player is valid
  if (player === undefined) {
    response.writeHead(400, create_header('text/plain'));
    response.end('Invalid ID specified.');
    return;
  }

  const did_reload = player.alive;
  player_controller.reload(player);
  response.writeHead(200, create_header('application/json'));
  const wrapper = { ...player }; // Make a clone of player
  wrapper.did_reload = did_reload;
  response.end(JSON.stringify(wrapper));
});

module.exports = router;