<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

	<xsl:output method="html"/>

	<xsl:template match="feeds">
		<table cellspacing="0" cellpadding="0" border="0" id="selectors-box" class="round-box">
			<tbody>
				<tr>
					<td class="s tl"/>
					<td class="s"/>
					<td class="s tr"/>
				</tr>
				<tr>
					<td class="s"/>
					<td class="c">
						<ul>
							<div class="sub-tree-header">Suscripciones</div>
							<table>
								<xsl:apply-templates select="feed"/>
							</table>
						</ul>
					</td>
					<td class="s"/>
				</tr>
				<tr>
					<td class="s bl"/>
					<td class="s"/>
					<td class="s br"/>
				</tr>
			</tbody>
		</table>
	</xsl:template>

	<xsl:template match="feed">
		<tr>
			<td class="menuIcon">
				<a class="link">
					<xsl:attribute name="onclick">
						doAction( 'actionCode=' + escape( 'F2' ) + '&amp;params=' + escape( 'feedId||#<xsl:value-of select="@id"/>' ), delFeedHandler, '' )
					</xsl:attribute>
					<img style="border:0" src="images/action_feed_del.png" title="Borrar feed" alt="Borrar feed"/>
				</a>
			</td>

			<td class="menuIcon">
				<a class="link">
					<xsl:attribute name="onclick">
						refreshFeed( <xsl:value-of select="@id"/> )
					</xsl:attribute>
					<img style="border:0" src="images/action_feed_refresh.png" title="Actualizar feed" alt="Actualizar feed"/>
				</a>
			</td>

			<td class="menuName">
				<a class="link">
					<xsl:attribute name="onclick">
						doAction('actionCode=' + escape( 'A5' ) + '&amp;params=' + escape( 'feedId||#<xsl:value-of select="@id"/>|||' ), bodyHandler, 'A' )
					</xsl:attribute>
					<xsl:value-of select="@name"/>
				</a>
			</td>
		</tr>
	</xsl:template>
</xsl:transform>