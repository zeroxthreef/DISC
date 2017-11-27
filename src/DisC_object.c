#include <stdio.h>
#include <stdlib.h>

#include <jansson.h>

#include "DisC_types.h"
#include "DisC_util.h"
#include "DisC_errors.h"
#include "DisC_object.h"

DisC_gateway_payload_t *DisC_object_GenerateGatewayPayload(DisC_session_t *session, char *jsonData)
{
  DisC_gateway_payload_t *payloadInternal = calloc(1, sizeof(DisC_gateway_payload_t));
  json_t *root = NULL;
  json_t *data = NULL;


  if(payloadInternal == NULL)
  {
    exit(1);
  }

  if(jsonData != NULL)
  {
    root = json_loads(jsonData, 0, NULL);

    payloadInternal->op = json_integer_value(json_object_get(root, "op"));
    //detecting type
    if(json_is_object(json_object_get(root, "d")))
      payloadInternal->dataType = DISC_EVENTDATA_TYPE_OBJECT;
    if(json_is_integer(json_object_get(root, "d")))
      payloadInternal->dataType = DISC_EVENTDATA_TYPE_INTEGER;
    if(json_is_boolean(json_object_get(root, "d")))
      payloadInternal->dataType = DISC_EVENTDATA_TYPE_BOOL;
    //================
    payloadInternal->d = DisC_strmkdup(json_dumps(json_object_get(root, "d"), 0));
    payloadInternal->s = json_integer_value(json_object_get(root, "s"));
    payloadInternal->t = DisC_strmkdup(json_string_value(json_object_get(root, "t")));
    //do the sub object creation
    /*
    switch(payloadInternal->op)
    {
      case DISC_OP_DISPATCH:

      break;
      case DISC_OP_HEARTBEAT:

      break;
      case DISC_OP_IDENTIFY:

      break;
      case DISC_OP_STATUS_UPDATE:

      break;
      case DISC_OP_VOICE_STATUS_UPDATE:

      break;
      case DISC_OP_VOICE_SERVER_PING:

      break;
      case DISC_OP_RESUME:

      break;
      case DISC_OP_RECONNECT:

      break;
      case DISC_OP_REQUEST_GUILD_MEMBERS:

      break;
      case DISC_OP_INVALID_SESSION:

      break;
      case DISC_OP_HELLO:

      break;
      case DISC_OP_HEARTBEAT_ACK:

      break;
    }
    */

    json_decref(root);

    return payloadInternal;
  }
  else
  {
    return NULL;
  }
}

DisC_channel_t *DisC_object_GenerateChannel(DisC_session_t *session, char *jsonData)
{
  DisC_channel_t *channelInternal = calloc(1, sizeof(DisC_channel_t));
  json_t *root = NULL;


  if(channelInternal == NULL)
  {
    exit(1);
  }

  if(jsonData != NULL)
  {
    root = json_loads(jsonData, 0, NULL);
    json_t *overwrites = json_object_get(root, "permission_overwrites");
    json_t *recipients = json_object_get(root, "recipients");

    channelInternal->id = DisC_strmkdup(json_string_value(json_object_get(root, "id")));
    channelInternal->type = json_integer_value(json_object_get(root, "type"));
    channelInternal->guild_id = DisC_strmkdup(json_string_value(json_object_get(root, "guild_id")));
    channelInternal->position = json_integer_value(json_object_get(root, "position"));
    //TODO OVERWRITE ARRAY OBJECTS
    if(overwrites != NULL)
    {
      channelInternal->permOverwritesCount = json_array_size(overwrites);
      DisC_overwrite_t *overwritesInternal = calloc(channelInternal->permOverwritesCount, sizeof(DisC_overwrite_t));
      unsigned long i;
      for(i = 0; i < channelInternal->permOverwritesCount; i++)
      {
        DisC_overwrite_t *temp = DisC_object_GenerateOverwrite(session, json_dumps(json_array_get(overwrites, i), 0));
        overwritesInternal[i] = *temp;
      }

      channelInternal->permission_overwrites = overwritesInternal;
    }
    else
    {
      channelInternal->permOverwritesCount = 0;
    }
    channelInternal->name = DisC_strmkdup(json_string_value(json_object_get(root, "name")));
    channelInternal->topic = DisC_strmkdup(json_string_value(json_object_get(root, "topic")));
    channelInternal->isNsfw = json_integer_value(json_object_get(root, "nsfw"));
    channelInternal->last_message_id = DisC_strmkdup(json_string_value(json_object_get(root, "last_message_id")));
    channelInternal->bitrate = json_integer_value(json_object_get(root, "bitrate"));
    channelInternal->user_limit = json_integer_value(json_object_get(root, "user_limit"));
    //TODO recipients
    if(recipients != NULL)
    {
      channelInternal->recipientCount = json_array_size(recipients);
      DisC_user_t *recipientsInternal = calloc(channelInternal->recipientCount, sizeof(DisC_user_t));
      unsigned long i;
      for(i = 0; i < channelInternal->recipientCount; i++)
      {
        DisC_user_t *temp = DisC_object_GenerateUser(session, json_dumps(json_array_get(recipients, i), 0));
        recipientsInternal[i] = *temp;
        //free(temp);//actually do it right
      }

      channelInternal->recipients = recipientsInternal;
    }
    else
    {
      channelInternal->recipientCount = 0;
    }
    channelInternal->icon = DisC_strmkdup(json_string_value(json_object_get(root, "icon")));
    channelInternal->owner_id = DisC_strmkdup(json_string_value(json_object_get(root, "owner_id")));
    channelInternal->application_id = DisC_strmkdup(json_string_value(json_object_get(root, "application_id")));
    channelInternal->parent_id = DisC_strmkdup(json_string_value(json_object_get(root, "parent_id")));
    channelInternal->last_pin_timestamp = DisC_strmkdup(json_string_value(json_object_get(root, "last_pin_timestamp")));

    json_decref(root);

    return channelInternal;
  }
  else
  {
    return NULL;
  }
}

DisC_message_t *DisC_object_GenerateMessage(DisC_session_t *session, char *jsonData)
{
  DisC_message_t *messageInternal = calloc(1, sizeof(DisC_message_t));
  json_t *root = NULL;
  json_t *mentions = NULL;
  json_t *roles = NULL;
  json_t *attachments = NULL;
  json_t *embeds = NULL;
  json_t *reactions = NULL;


  if(messageInternal == NULL)
  {
    exit(1);
  }

  if(jsonData != NULL)
  {
    root = json_loads(jsonData, 0, NULL);
    DisC_user_t *account = NULL;
    mentions = json_object_get(root, "mentions");
    roles = json_object_get(root, "mention_roles");
    attachments = json_object_get(root, "attachments");
    embeds = json_object_get(root, "embeds");
    reactions = json_object_get(root, "reactions");

    messageInternal->id = DisC_strmkdup(json_string_value(json_object_get(root, "id")));
    messageInternal->channel_id = DisC_strmkdup(json_string_value(json_object_get(root, "channel_id")));

    account = DisC_object_GenerateUser(session, json_dumps(json_object_get(root, "author"), 0));
    messageInternal->author = *account;

    messageInternal->content = DisC_strmkdup(json_string_value(json_object_get(root, "content")));
    messageInternal->timestamp = DisC_strmkdup(json_string_value(json_object_get(root, "timestamp")));
    messageInternal->edited_timestamp = DisC_strmkdup(json_string_value(json_object_get(root, "edited_timestamp")));
    messageInternal->tts = json_boolean_value(json_object_get(root, "tts"));
    messageInternal->mention_everyone = json_boolean_value(json_object_get(root, "mention_everyone"));
    //=============//TODO fix. This wasnt working
    if(mentions != NULL)
    {
      //printf("%s\n", json_dumps(mentions, JSON_INDENT(2)));
      messageInternal->mentionCount = json_array_size(mentions);
      DisC_user_t *mentionsInternal = calloc(messageInternal->mentionCount, sizeof(DisC_user_t));
      unsigned long i;
      for(i = 0; i < messageInternal->mentionCount; i++)
      {
        DisC_user_t *temp = DisC_object_GenerateUser(session, json_dumps(json_array_get(mentions, i), 0));
        mentionsInternal[i] = *temp;
        //free(temp);//actually do it right
      }

      messageInternal->mentions = mentionsInternal;
    }
    else
    {
      messageInternal->mentionCount = 0;
    }

    if(roles != NULL)//TODO fix this. It segfaults on access
    {
      //printf("%s\n", json_dumps(roles, JSON_INDENT(2)));
      messageInternal->roleCount = json_array_size(roles);
      messageInternal->roles = calloc(messageInternal->roleCount, DISC_SNOWFLAKE_ALLOCSIZE * sizeof(char));
      unsigned long i;
      for(i = 0; i < messageInternal->roleCount; i++)
      {
        messageInternal->roles[i] = DisC_strmkdup(json_string_value(json_array_get(roles, i)));
        //free(temp);//actually do it right
      }
      //if(messageInternal->roleCount> 1)
      //{
        //printf("%s\n%s\n", messageInternal->roles[0], messageInternal->roles[1]);
      //}
    }
    else
    {
      messageInternal->roleCount = 0;
    }

    if(attachments != NULL)
    {
      //printf("%s\n", json_dumps(mentions, JSON_INDENT(2)));
      messageInternal->attachmentCount = json_array_size(attachments);
      DisC_attachment_t *attachmentInternal = calloc(messageInternal->attachmentCount, sizeof(DisC_attachment_t));
      unsigned long i;
      for(i = 0; i < messageInternal->attachmentCount; i++)
      {
        DisC_attachment_t *temp = DisC_object_GenerateAttachment(session, json_dumps(json_array_get(attachments, i), 0));
        attachmentInternal[i] = *temp;
        //free(temp);//actually do it right
      }

      messageInternal->attatchments = attachmentInternal;
    }
    else
    {
      messageInternal->attachmentCount = 0;
    }

    if(embeds != NULL)
    {
      //printf("%s\n", json_dumps(mentions, JSON_INDENT(2)));
      messageInternal->embedCount = json_array_size(embeds);
      DisC_embed_t *embedInternal = calloc(messageInternal->embedCount, sizeof(DisC_embed_t));
      unsigned long i;
      for(i = 0; i < messageInternal->embedCount; i++)
      {
        DisC_embed_t *temp = DisC_object_GenerateEmbed(session, json_dumps(json_array_get(embeds, i), 0));
        embedInternal[i] = *temp;
        //free(temp);//actually do it right
      }

      messageInternal->embeds = embedInternal;
    }
    else
    {
      messageInternal->embedCount = 0;
    }

    if(reactions != NULL)
    {
      //printf("%s\n", json_dumps(mentions, JSON_INDENT(2)));
      messageInternal->reactionCount = json_array_size(reactions);
      DisC_reaction_t *reactionInternal = calloc(messageInternal->reactionCount, sizeof(DisC_reaction_t));
      unsigned long i;
      for(i = 0; i < messageInternal->reactionCount; i++)
      {
        DisC_reaction_t *temp = DisC_object_GenerateReaction(session, json_dumps(json_array_get(reactions, i), 0));
        reactionInternal[i] = *temp;
        //free(temp);//actually do it right
      }

      messageInternal->reactions = reactionInternal;
    }
    else
    {
      messageInternal->reactionCount = 0;
    }


    messageInternal->nonce = DisC_strmkdup(json_string_value(json_object_get(root, "nonce")));
    messageInternal->pinned = json_boolean_value(json_object_get(root, "pinned"));
    messageInternal->webhook_id = DisC_strmkdup(json_string_value(json_object_get(root, "webhook_id")));
    messageInternal->type = json_integer_value(json_object_get(root, "type"));

    json_decref(root);

    return messageInternal;
  }
  else
  {
    return NULL;
  }
}

DisC_user_t *DisC_object_GenerateUser(DisC_session_t *session, char *jsonData)
{
  DisC_user_t *userInternal = calloc(1, sizeof(DisC_user_t));
  json_t *root = NULL;


  if(userInternal == NULL)
  {
    exit(1);
  }

  if(jsonData != NULL)
  {
    root = json_loads(jsonData, 0, NULL);


    userInternal->id = DisC_strmkdup(json_string_value(json_object_get(root, "id")));
    userInternal->username = DisC_strmkdup(json_string_value(json_object_get(root, "username")));
    userInternal->discriminator = DisC_strmkdup(json_string_value(json_object_get(root, "discriminator")));
    userInternal->avatar = DisC_strmkdup(json_string_value(json_object_get(root, "avatar")));
    userInternal->bot = json_boolean_value(json_object_get(root, "bot"));
    userInternal->mfa_enabled = json_boolean_value(json_object_get(root, "mfa_enabled"));
    userInternal->verified = json_boolean_value(json_object_get(root, "verified"));
    userInternal->email = DisC_strmkdup(json_string_value(json_object_get(root, "email")));

    json_decref(root);

    return userInternal;
  }
  else
  {
    return NULL;
  }
}

DisC_overwrite_t *DisC_object_GenerateOverwrite(DisC_session_t *session, char *jsonData)
{
  DisC_overwrite_t *overwriteInternal = calloc(1, sizeof(DisC_overwrite_t));
  json_t *root = NULL;


  if(overwriteInternal == NULL)
  {
    exit(1);
  }

  if(jsonData != NULL)
  {
    root = json_loads(jsonData, 0, NULL);

    overwriteInternal->id = DisC_strmkdup(json_string_value(json_object_get(root, "id")));
    overwriteInternal->type = DisC_strmkdup(json_string_value(json_object_get(root, "type")));
    overwriteInternal->allow = json_integer_value(json_object_get(root, "allow"));
    overwriteInternal->deny = json_integer_value(json_object_get(root, "deny"));

    json_decref(root);
    return overwriteInternal;
  }
  else
  {
    return NULL;
  }
}

DisC_role_t *DisC_object_GenerateRole(DisC_session_t *session, char *jsonData)
{
  DisC_role_t *roleInternal = calloc(1, sizeof(DisC_role_t));
  json_t *root = NULL;

  if(roleInternal == NULL)
  {
    exit(1);
  }

  if(jsonData != NULL)
  {
    root = json_loads(jsonData, 0, NULL);

    roleInternal->id = DisC_strmkdup(json_string_value(json_object_get(root, "id")));
    roleInternal->name = DisC_strmkdup(json_string_value(json_object_get(root, "name")));
    roleInternal->color = json_integer_value(json_object_get(root, "color"));
    roleInternal->hoist = json_boolean_value(json_object_get(root, "hoist"));
    roleInternal->position = json_integer_value(json_object_get(root, "position"));
    roleInternal->permissions = json_integer_value(json_object_get(root, "permissions"));
    roleInternal->managed = json_boolean_value(json_object_get(root, "managed"));
    roleInternal->mentionable = json_boolean_value(json_object_get(root, "mentionable"));

    json_decref(root);

    return roleInternal;
  }
  else
  {
    return NULL;
  }
}

DisC_attachment_t *DisC_object_GenerateAttachment(DisC_session_t *session, char *jsonData)
{
  DisC_attachment_t *attachmentInternal = calloc(1, sizeof(DisC_attachment_t));
  json_t *root = NULL;


  if(attachmentInternal == NULL)
  {
    exit(1);
  }

  if(jsonData != NULL)
  {
    root = json_loads(jsonData, 0, NULL);

    attachmentInternal->id = DisC_strmkdup(json_string_value(json_object_get(root, "id")));
    attachmentInternal->filename = DisC_strmkdup(json_string_value(json_object_get(root, "filename")));
    attachmentInternal->size = json_integer_value(json_object_get(root, "size"));
    attachmentInternal->url = DisC_strmkdup(json_string_value(json_object_get(root, "url")));
    attachmentInternal->proxy_url = DisC_strmkdup(json_string_value(json_object_get(root, "proxy_url")));
    attachmentInternal->height = json_integer_value(json_object_get(root, "height"));
    attachmentInternal->width = json_integer_value(json_object_get(root, "width"));


    json_decref(root);

    return attachmentInternal;
  }
  else
  {
    return NULL;
  }
}

DisC_embed_t *DisC_object_GenerateEmbed(DisC_session_t *session, char *jsonData)
{
  DisC_embed_t *embedInternal = calloc(1, sizeof(DisC_embed_t));
  json_t *root = NULL;
  json_t *footer = NULL;
  json_t *image = NULL;
  json_t *thumbnail = NULL;
  json_t *video = NULL;
  json_t *provider = NULL;
  json_t *author = NULL;
  json_t *fields = NULL;


  if(embedInternal == NULL)
  {
    exit(1);
  }

  if(jsonData != NULL)
  {
    root = json_loads(jsonData, 0, NULL);
    footer = json_object_get(root, "footer");
    image = json_object_get(root, "image");
    thumbnail = json_object_get(root, "thumbnail");
    video = json_object_get(root, "video");
    provider = json_object_get(root, "provider");
    author = json_object_get(root, "author");
    fields = json_object_get(root, "fields");

    embedInternal->title = DisC_strmkdup(json_string_value(json_object_get(root, "title")));
    embedInternal->type = DisC_strmkdup(json_string_value(json_object_get(root, "type")));
    embedInternal->description = DisC_strmkdup(json_string_value(json_object_get(root, "description")));
    embedInternal->url = DisC_strmkdup(json_string_value(json_object_get(root, "url")));
    embedInternal->timestamp = DisC_strmkdup(json_string_value(json_object_get(root, "timestamp")));
    embedInternal->color = json_integer_value(json_object_get(root, "color"));
    //TODO footer
    //TODO image
    //TODO thumbnail
    //TODO video
    //TODO provider
    //TODO author
    //TODO field array

    json_decref(root);

    return embedInternal;
  }
  else
  {
    return NULL;
  }
}

DisC_reaction_t *DisC_object_GenerateReaction(DisC_session_t *session, char *jsonData)
{
  DisC_reaction_t *reactionInternal = calloc(1, sizeof(DisC_reaction_t));
  json_t *root = NULL;


  if(reactionInternal == NULL)
  {
    exit(1);
  }

  if(jsonData != NULL)
  {
    root = json_loads(jsonData, 0, NULL);

    reactionInternal->count = json_integer_value(json_object_get(root, "count"));
    reactionInternal->me = json_boolean_value(json_object_get(root, "me"));
    //TODO emoji

    json_decref(root);

    return reactionInternal;
  }
  else
  {
    return NULL;
  }
}
